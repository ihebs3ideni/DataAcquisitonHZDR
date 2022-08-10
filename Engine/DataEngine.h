//
// Created by IHEB on 24/06/2022.
//

#ifndef HZDR_DATA_ACQUISITION_DATAENGINE_H
#define HZDR_DATA_ACQUISITION_DATAENGINE_H
#include "ChannelData.h"
#include "../DataStructures/Config.h"
#include <vector>

#include "../Libraries/Numerical_Magic/Numerical_magic.h"
void dummy_cb(ENGINE::JOB<ENGINE::DataFrame<float, NM::COMPLEX>> job){
//    std::cout << job.ID << ": ";
//    for (auto& e:job.data){
//        std::cout << e.timestamp << "|" << e.data[0] << " ; ";
//    }
//    std::cout << "called\n";
    job.return_result(job.data.at(0));

}


namespace ENGINE{
    class DataEngine{
    public:
        DataEngine(Config& config, ENGINE::CHANNELS_OP_MODE opMode_,
                   std::shared_ptr<ENGINE::LOGGER> logger=std::shared_ptr<ENGINE::LOGGER>(),
                   std::shared_ptr<task_system> tp=std::shared_ptr<task_system>()):
        configurations_(config), thread_pool(tp), opMode(opMode_), logger_(logger){}
        ~DataEngine() {
            stop_all();
            for (auto& ch: regularOldChannels)
                delete ch.second;
            for (auto& ref: refChannels)
                delete ref.second;
        }

        void initialize(const std::vector<std::function<void(const ENGINE::JOB<ENGINE::DataFrame<float, NM::COMPLEX>>&)>>& tasks)try{
            //double run to make sure the references are created before the other channels
            for (auto& ch: configurations_.channels.analog_channels)
                if (ch.is_ref){
                    refChannels.emplace( ch.id,
                            new ENGINE::RefChannel_Buffer<ENGINE::DataFrame<float, NM::COMPLEX>>(
                                    ch.id, configurations_.app.buffer_size, configurations_.app.block_size,
                                    configurations_.app.sliding_window_size, configurations_.sampling.timebase*1e-9,
                                    ch.coil_type, logger_, thread_pool) //convert timebase to seconds
                    );

                }
            std::map<int, ENGINE::RefChannel_Buffer<ENGINE::DataFrame<float, NM::COMPLEX>>*>* ref_ptr;
            if (refChannels.size()>0)
                ref_ptr = &refChannels;
            else
                ref_ptr = nullptr;
            for (auto& ch: configurations_.channels.analog_channels){
                if (ch.is_ref)
                    continue;
                regularOldChannels.emplace( ch.id,
                        new ENGINE::NormalChannel_Buffer<ENGINE::DataFrame<float, NM::COMPLEX>>(
                                ch.id, configurations_.app.buffer_size, configurations_.app.block_size,
                                configurations_.app.sliding_window_size, configurations_.sampling.timebase*1e-9, //convert timebase to seconds
                                std::rint(configurations_.app.buffer_size/configurations_.app.block_size), ch.coil_type,
                                ref_ptr ,logger_, thread_pool)
                        );
            }
            for (auto& cb:tasks)
                this->add_task(cb);
            refChannels.begin()->second->set_operation_mode(opMode); //this is shared by all channels as a static variable
        }
        catch(const ENGINE::CHANNEL_ERROR& err){
            std::cout << "ERROR IN ENGINE::DataEngine::initialize: " << err.what() << "\n";
        }

        void start_all(){
            for (auto& ch: regularOldChannels)
                ch.second->run();
            for (auto& ref: refChannels)
                ref.second->run();

        }

        void stop_all(){
            for (auto& ch: regularOldChannels)
                ch.second->stop();
            for (auto& ref: refChannels)
                ref.second->stop();
        }

        void start_one(unsigned channel_id){
            if (configurations_.channels.analog_channels[channel_id].is_ref)
                refChannels.at(channel_id)->run();
            else regularOldChannels.at(channel_id)->run();
        }

        void stop_one(unsigned channel_id){
            if (configurations_.channels.analog_channels[channel_id].is_ref)
                refChannels.at(channel_id)->stop();
            else regularOldChannels.at(channel_id)->stop();
        }

        void add_task(std::function<void(const ENGINE::JOB<ENGINE::DataFrame<float, NM::COMPLEX>>&)> cb, int channel_index = -1){
            ///if channel_index ist not overloaded or is -1 the task will be added to all channels
            ///this assumes the channel_index provided is correct or -1
            if (channel_index == -1)
                for(auto& ch: regularOldChannels)
                    ch.second->add_task(cb);
            else
//                if(regularOldChannels.size()>index)
                regularOldChannels.at(channel_index)->add_task(cb);
        }

        void delete_task(unsigned task_index, int channel_index = -1){
            ///if channel_index ist not overloaded or is -1 the task will be added to all channels
            ///this assumes the channel_index provided is correct or -1
            if (channel_index == -1)
                for(auto& ch: regularOldChannels)
                    ch.second->delete_task(task_index);
            else
//            if(regularOldChannels.size()>channel_index)
                regularOldChannels.at(channel_index)->delete_task(task_index);
        }

        void add_filter(Dsp::Filter* filter_, int channel_index = -1){
            if (regularOldChannels.count(channel_index) == 0){
                refChannels.at(channel_index)->add_filter(filter_);
                return;
            }
            regularOldChannels.at(channel_index)->add_filter(filter_);
        }

        void add_filter(){
            /***overloaded to create default filters***/
            for(auto& ch: regularOldChannels)
                ch.second->add_filter(create_default_filter());
            for(auto& ch: refChannels)
                ch.second->add_filter(create_default_filter());
        }

        void add_logger(std::shared_ptr<ENGINE::LOGGER> log){
            logger_ = log;
            for (auto& ch: regularOldChannels)
                ch.second->add_logger(logger_);
            for (auto& ref: refChannels)
                ref.second->add_logger(logger_);
        }

        std::map<int, ENGINE::NormalChannel_Buffer<ENGINE::DataFrame<float, NM::COMPLEX>>*> regularOldChannels;
        std::map<int, ENGINE::RefChannel_Buffer<ENGINE::DataFrame<float, NM::COMPLEX>>*> refChannels;
    private:
        Config& configurations_;
        std::shared_ptr<task_system> thread_pool;
        std::shared_ptr<ENGINE::LOGGER> logger_;
        ENGINE::CHANNELS_OP_MODE opMode;
    };
}

void test_data_engine(){
    Config cg("C:\\Users\\User\\Desktop\\HZDR\\hzdr_tasler_data_acquisition\\Examples\\tasler_config.json");
    std::cin.get();
    //create a shared pointer directly instead of passing the pointer of a static variable to std::shared:ptr
    //to avoid copyin because shared_pointer has to have ownership of the object passed to it
    auto tp = std::make_shared<task_system>(cg.app.thread_count);
    auto logger = std::make_shared<ENGINE::LOGGER>(cg, ENGINE::ASCII);
    ENGINE::DataEngine db(cg, ENGINE::SHARED, logger, tp);
    db.initialize({ENGINE::clean_reference_signal, ENGINE::demodulate}); //
    db.add_filter();
    db.start_all();
    auto signal_ = NM::sin_(1, 1, 16, 0, cg.sampling.timebase*1e-9);
    testing_timer.setInterval([&]{
        for (auto& ref: db.refChannels)
            ref.second->start_tracking();
        for(auto& ch: db.regularOldChannels)
            ch.second->start_tracking();
    }, (int) 5*1e6);
    unsigned long ts{0};
    testing_timer.setInterval([&]{
        try{
            for (unsigned i=0; i<1024; i++){
                for (auto& b: db.refChannels){
                    b.second->put((float)ts/100, 1000*signal_[ts%signal_.size()]);//1000*signal_[ts%signal_.size()]
                }
                for (auto& b: db.regularOldChannels)
                    b.second->put((float)ts/100, 1000*signal_[ts%signal_.size()]);//1000*signal_[ts%signal_.size()]
                ts++;
            }

        } catch(const std::exception& ex){
            std::cout << "ERROR IN Generator: " << ex.what() << "\n";
        }
    }, (int)1);
    std::cin.get();
    db.stop_all();
}



#endif //HZDR_DATA_ACQUISITION_DATAENGINE_H
