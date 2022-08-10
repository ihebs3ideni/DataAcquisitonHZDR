//
// Created by IHEB on 29/06/2022.
//

#ifndef HZDR_DATA_ACQUISITION_LTTENGINE_H
#define HZDR_DATA_ACQUISITION_LTTENGINE_H
#include "../Base/BaseProcess.h"
#include "../IOs/LTT24/ltt24_whisperer.h"
#include "../IOs/Files/Logger.h"
#include "DataEngine.h"

namespace ENGINE{
    class LTTEngine: public Process{
    public:
        LTTEngine(unsigned int process_id, Config& cfg, std::shared_ptr<ENGINE::DataEngine> db_ref) : Process(process_id), cfg_(cfg), ltt(cfg),
            data_base(db_ref)
        {
            if (!db_ref)
                throw ENGINE::CHANNEL_ERROR("NO DATAENGINE POINTER PROVIDED TO LTTENGINE!!!!");
            std::cout << ltt.id_ << "\n";

        }

        ~LTTEngine(){
            if (this->running){
                this->shut_down_ = true;
            }else{
                ltt.deinitialize();
            }
            delete raw_buffer;
        }

        void test_mode(bool state = true){
            test_mode_ = state;

        }

        void start_tracker(int interval_mrs = (int) 5*1e6){
//            Timer testing_timer;
            testing_timer.setInterval([this]{
                for (auto& ref: data_base->refChannels)
                    std::cout << ref.second->start_tracking();
                for(auto& ch: data_base->regularOldChannels)
                    std::cout << ch.second->start_tracking();
            }, interval_mrs);
        }

        void start_tracker(std::function<void(const std::string& txt)> custom_printer, int interval_mrs = (int) 5*1e6){
//            Timer testing_timer;
            testing_timer.setInterval([this, custom_printer]{
                for (auto& ref: data_base->refChannels)
                    custom_printer(ref.second->start_tracking());
                for(auto& ch: data_base->regularOldChannels)
                    custom_printer(ch.second->start_tracking());
            }, interval_mrs);
        }


        void initialize()try{

            //before init
            ltt.set_devices_order();

            //initialize device internally
            if (test_mode_){
//                start_tracker();
                printf("NUMBER OF EMULATED DEVICE: %d\n", cfg_.devices_order.size());
                ltt.emulate(cfg_.devices_order.size());
            }

            ltt.initialize();

            //after init and before parameter upload
            ltt.set_devices_op_mode();

            ltt.upload_parameter();

            //set conversion factors
            conversion_factors = ltt.getChannelsConversionFactors();


            //reserve main buffer data !!!deleted in the destructor
            raw_buffer = new unsigned char[cfg_.sampling.chunk_sequence_cnt * ltt.numberOfDevices *
                                           ltt.numberOfChannels * ltt.sample_size];
            if (! raw_buffer)
                throw LTTError("MEAS: memory allocatin error!\n");

        }catch(LTTError& err){
            std::cerr << "LTT ERROR: INITIALIZE: " << err.what() << "\n";
        }

        void run() override {
            this->shut_down_ = false;
            this->running = false;
            this->task_thread.push_back(std::make_shared<std::thread>([this]() {
                this->running = true;
                //we assume the initialize method is already called on this
                this->ltt.start_devices();
                this->ltt.check_trigger(50);
                while(true){
                    if(this->shut_down()){ // if shut_down is true raw data is an empty optional
                        this->running = false;
                        ltt.stop_devices();
                        ltt.deinitialize();
                        return;
                    }
                    this->read_data(); //this interacts with the data base threads, look out for race conditions or queues being too slow
                }
            }));
            for (auto& t: this->task_thread)
                t->detach();
        }

        void stop() override{
            this->shut_down_ = true;


        }

    private:
        ///RESPONSIBLE for reading, interpreting and pushing the data to the database
        void read_data() {
            LTTSyncCorrection(1);

            auto status = ltt.start_data_stream(raw_buffer);
            if (status !=0)
            {
                printf( "MEAS: error initiating transfer %d\n", status );
                return;
            }
//            float resolution = ltt.get_time_resolution();
//            auto signal_ = NM::sin_(1, 1, 8, 0, cfg_.sampling.timebase*1e-9);
            //wait for transfer to finsish
            int xferbytes = ltt.wait_for_transfer();


            /// go through sequence
            /// -----------------------------------------------------------------------------------
            /// handle received chunk

            /// data are organized in groups af sequences of active channels.
            /// example:
            ///
            /// channels 0, 1, 3, 7 are active.
            /// the buffer will look like:
            ///
            ///   |           0           |           1           |           2           |...
            ///   |[ 0 ] [ 1 ] [ 3 ] [ 7 ]|[ 0 ] [ 1 ] [ 3 ] [ 7 ]|[ 0 ] [ 1 ] [ 3 ] [ 7 ]|...
            int32_t* p_smpl;
            int xfer_seq_cnt{0}; //to avoid overflow
            float resolution = ltt.get_time_resolution();
            unsigned seq_len = cfg_.channels.analog_channels.size();
            double voltage;
            for (int seq_idx = 0; seq_idx<(seq_len * xferbytes); seq_idx+=seq_len){
                //ref channels need to be filled before regular channels so the
                // promise isn't acquired before the data is ready
                for (auto& b: data_base->refChannels){
//                    std::cout << "data index: " << seq_idx + b.first << "\n";
                    p_smpl = ( (int32_t *)raw_buffer ) + seq_idx + b.first;
                    voltage = (double )(*p_smpl * conversion_factors[b.first] / 1000) ; //convert adc value to voltage in volts
                    b.second->put(timestep_counter, voltage);
                }
                for (auto& b: data_base->regularOldChannels){
//                    std::cout << "data index: " << seq_idx + b.first << "\n";
                    p_smpl = ( (int32_t *)raw_buffer ) + seq_idx + b.first;
                    voltage = (double )(*p_smpl * conversion_factors[b.first] / 1000) ; //convert adc value to voltage in volts
                    b.second->put(timestep_counter, voltage);
                }
                timestep_counter += resolution;
//                printf("t=%f\t", timestep_counter);
            }
            xfer_seq_cnt += xferbytes;


        }
        LTT24 ltt;
        Config& cfg_;
        std::shared_ptr<ENGINE::DataEngine> data_base;
        unsigned char* raw_buffer;
        float timestep_counter{0.};
        std::vector<double> conversion_factors;
//        unsigned long xfer_seq_cnt{0}; //to avoid overflow

        bool test_mode_{false};
    };
}

void test_lttEngine(){
    Config cg("C:\\Users\\User\\Desktop\\HZDR\\hzdr_tasler_data_acquisition\\Examples\\tasler_config.json");
    //create a shared pointer directly instead of passing the pointer of a static variable to std::shared:ptr
    //to avoid copyin because shared_pointer has to have ownership of the object passed to it
    auto tp = std::make_shared<task_system>(cg.app.thread_count);
    auto logger = std::make_shared<ENGINE::LOGGER>(cg, ENGINE::ASCII);
//    logger->run();
    //TODO: CHECK IF DATA BLOCK IS REACHING THE DEMODULATION FUNCTION AS IT SHOULD
    auto db = std::make_shared<ENGINE::DataEngine>(cg, ENGINE::ASYNCHRONOUS, logger, tp);
    db->initialize({ENGINE::clean_reference_signal, ENGINE::demodulate});
    db->add_filter();
    db->start_all();

    ENGINE::LTTEngine ltt_en(-560, cg, db);
    ltt_en.test_mode();
    ltt_en.initialize();
    printf("START?");
    std::cin.get();
    ltt_en.run();

    std::cin.get();
    ltt_en.stop();
    db->stop_all();
//    logger->stop();
    std::cout << "BYE\n";
}

#endif //HZDR_DATA_ACQUISITION_LTTENGINE_H
