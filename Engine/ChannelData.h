//
// Created by iheb on 13.05.2022.
//

#ifndef HZDR_DATA_ACQUISITION_CHANNELDATA_H
#define HZDR_DATA_ACQUISITION_CHANNELDATA_H

#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>
#include <memory>
#include <functional>
#include <atomic>
#include <vector>
#include <future>


#include "../Base/BaseProcess.h"

#include "../DataStructures/RingBuffer.h"
#include "../Libraries/Numerical_Magic/Numerical_magic.h"
#include "../Tools/Timer.h"
#include "../Tools/Filter_factory.h"
#include "../Thread_Pool/Tasking_system.h"
#include "../Models/Coils.h"
#include "../IOs/Files/Logger.h"

#include "Tasks.h"
#include "states.h"


Timer testing_timer;


namespace ENGINE {

    template<typename T=ENGINE::DataFrame<float, NM::COMPLEX>>
    class BASE_CHANNEL_BUFFER : public Process {
    public:
        BASE_CHANNEL_BUFFER(int id, unsigned buffer_size, //std::function<void(ENGINE::JOB<T>)> br_cb,
                            unsigned block_size_, unsigned slidingWindow_size,
                            double timebase_, //,unsigned results_buff_size = 100
                            const std::string &coil_type_,
                            std::shared_ptr<ENGINE::LOGGER> logger=std::shared_ptr<ENGINE::LOGGER>(),
                            std::shared_ptr<task_system> tp=std::shared_ptr<task_system>()) : Process(id),
                                                                                block_size(block_size_),
                                                                                capacity(buffer_size), logger_(logger),
                                                                                data_Buffer(simple_RingBuffer<T>(
                                                                                        buffer_size)),
                                                                                thread_pool(tp), //logger_(logger),
                //output_Buffer(simple_RingBuffer<T>(results_buff_size)),
                                                                                coil_type(coil_type_),
                                                                                sliding_window_size(slidingWindow_size),
                                                                                timebase(timebase_) {
        }

        BASE_CHANNEL_BUFFER(BASE_CHANNEL_BUFFER<T> &&bc) : Process(bc.ID),
                                                           block_size(bc.block_size),
                                                           capacity(bc.capacity), //callback(std::move(br_cb)),
                                                           data_Buffer(std::move(bc.data_Buffer)),
                                                           thread_pool(bc.thread_pool), logger_(bc.logger_),
                                                           coil_type(bc.coil_type),
                                                           sliding_window_size(bc.sliding_window_size) {}


        simple_RingBuffer<T> data_Buffer;
        std::string coil_type;


        void put(float timestamp, double data, bool print_ = false) {
            this->df.timestamp = timestamp;
            this->df.data = NM::COMPLEX(data);
            this->data_Buffer.push_back(this->df);
            this->block_element_count++;
            if(this->logger_)
                this->logger_->log_raw(this->ID, this->df);
            if (this->data_to_process_available()) {
//                printf("BLOCK READY\n");
                switch (this->opMode) {
                    case (SHARED):
                        this->shared_queue->push([this] { this->process(); }); //push job to shared worker
                        break;
                    case (ASYNCHRONOUS): {
                        if (this->thread_pool){
                            this->thread_pool->async_([this] { this->process(); });
                            break;
                        }
                        throw ENGINE::CHANNEL_ERROR("ASYNCHRONOUS MODE IS SET BUT NO THREAD-POOL IS PROVIDED\n");

                    }

                    case (SYNCHRONOUS): {
                        this->process(print_);
                        break;
                    }

                    default:
                        this->process(); //process in the same thread as called
                }
            }

        }

        void run() override {
            switch (opMode) {
                case (SHARED):
                    std::cout << "SHARED OPERATION MODE SET\n";
                    this->run_shared();
                    return;
                case (ASYNCHRONOUS):
                    std::cout << "ASYNCHRONOUS OPERATION MODE SET\n";
                    return;
                case SYNCHRONOUS:
                    std::cout << "SYNCHRONOUS OPERATION MODE SET\n";
                    return;
            }
        }

        void run_shared() {
            //used instead of threadpool with 1 thread to free threadpool resources and enforece one thread deals with
            //data processing.
            this->shut_down_ = false;
            this->running = false;
            if (!this->shared_queue)
                this->shared_queue = std::unique_ptr<notification_queue>(new notification_queue);
            if (!this->shared_worker) {
                std::cout << "STARTING SHARED WORKER THREAD\n";
                this->shared_worker = std::make_shared<std::thread>([this]() {
                    while (true) {
                        std::function<void()> f;
                        this->shared_queue->pop(f); //BLOCKING
                        if (this->shut_down()) { // if shut_down
                            std::cout << "SHARED WORKER THREAD SHUT DOWN\n";
                            this->running = false;
                            return;
                        }
                        f();
                    }
                });
                this->shared_worker->detach();
            }

        }

        void stop() override {
            this->shut_down_ = true;
            if (this->shared_queue)
                this->shared_queue->push([] {}); //to wake worker up in case queue is empty
        }


        void set_block_size(unsigned new_block_size) {
            if (new_block_size < this->sliding_window_size)
                throw ENGINE::CHANNEL_ERROR("provided block size less than window size!!!");
            this->block_size = new_block_size;
        }

        void set_window_size(unsigned new_window_size) {
            if (new_window_size > this->block_size)
                throw ENGINE::CHANNEL_ERROR("provided window size greater than block size!!!");
            this->sliding_window_size = new_window_size;
        }


        void set_timebase(unsigned tb) {
            this->timebase = tb;
        }

        void add_filter(Dsp::Filter *filter_) {
            /***takes ownership of filter pointer***/
            this->filter = std::unique_ptr<Dsp::Filter>(filter_);
        }


        void add_task(std::function<void(const ENGINE::JOB<T> &)> task) {
            /***tasks are meant to be ran in the same order they r put in the queue
             tasks must write the new values either by calling the return_results method
             or overwriting the job object with new references      ***/
            this->tasks.push_back(std::move(task));
        }

        void delete_task(unsigned index) {
            this->tasks.erase(this->tasks.begin() + index);
        }

        unsigned get_capacity() const {
            return this->capacity;
        }

        void set_operation_mode(enum ENGINE::CHANNELS_OP_MODE op_mode) {
            opMode = op_mode;
        }

        virtual std::string start_tracking() {return "";}

        virtual void add_logger(std::shared_ptr<ENGINE::LOGGER> log){}

    protected:
        virtual void process(bool print_ = false) {};

        bool data_to_process_available() {
            if (this->block_element_count >= (int) this->block_size) {
                this->block_element_count -= this->sliding_window_size;
                return true;
            }
            return false;
        }

        std::shared_ptr<ENGINE::LOGGER> logger_;
        std::unique_ptr<Dsp::Filter> filter;
        ENGINE::JOB<T> job; //to avoid creating new job strucs everytime we can reuse the same object
        unsigned block_size, capacity, sliding_window_size, block_element_count{0};
        double timebase{0.}; //in s
        std::vector<std::function<void(const ENGINE::JOB<T> &)>> tasks;
        T df; //dummy data frame reused to avoid creating new objects
//        std::unique_ptr<std::promise<void>> BLOCK_PROMISE;
//        std::future<void> BLOCK_FUTURE;
        std::shared_ptr<task_system> thread_pool;
        inline static std::shared_ptr<std::thread> shared_worker;
        inline static std::unique_ptr<notification_queue> shared_queue;
        inline static enum ENGINE::CHANNELS_OP_MODE opMode{ENGINE::SHARED};

    };




//##################################################################################


//##################################################################################
    template<typename T=ENGINE::DataFrame<float, NM::COMPLEX>>
    class RefChannel_Buffer : public ENGINE::BASE_CHANNEL_BUFFER<T> {
    public:
        RefChannel_Buffer(int id, unsigned buffer_size, unsigned block_size_,
                          unsigned slidingWindow_size, double timebase_,
                          const std::string &coil_type, std::shared_ptr<ENGINE::LOGGER> logger=std::shared_ptr<ENGINE::LOGGER>(),
                          std::shared_ptr<task_system> tp=std::shared_ptr<task_system>()
        ) : BASE_CHANNEL_BUFFER<T>(id, buffer_size, block_size_, slidingWindow_size,
                                   timebase_, coil_type, logger, tp) {

            if (this->logger_)
                this->logger_->subscribe(this->ID, true);
            std::cout << this->ID << ";" << "IS REF; " << this->sliding_window_size << ";" << this->block_size << "\n";
            if (this->sliding_window_size > this->block_size)
                throw ENGINE::CHANNEL_ERROR(
                        std::string("REFCHANNEL: the size of the Siding Window is greater than the size of block!!!"
                                    + std::to_string(this->sliding_window_size) + ";"
                                    + std::to_string(this->block_size)));
        }

        // move semantics
        RefChannel_Buffer(RefChannel_Buffer<T> &&ub)
                : BASE_CHANNEL_BUFFER<T>(std::move(ub)) {
            if (this->logger_)
                this->logger_->subscribe(this->ID, true);
            std::cout << this->ID << ";" << this->sliding_window_size << ";" << this->block_size << "\n";
            if (this->sliding_window_size > this->block_size)
                throw ENGINE::CHANNEL_ERROR(
                        std::string("REFCHANNEL: the size of the Siding Window is greater than the size of block!!!"
                                    + std::to_string(this->sliding_window_size) + ";"
                                    + std::to_string(this->block_size)));

        }


        std::string start_tracking() override {
            std::ostringstream streamObj;
            streamObj << "REF Channel " << this->ID << ": Size: " << this->data_Buffer.size()
                      << " Frequency: " << this->signal_frequency << "\n";
            return streamObj.str();

        }

        std::vector<T> get_block() {
            ref_ready:
            if (this->reference_data_future.valid()) {
                this->reference_data_future.wait();
                return this->data_block;
            } else {
                std::cout << "waiting for future to be valid\n";
                goto ref_ready;
            }
            //            return this->data_block;
        }

        void add_logger(std::shared_ptr<ENGINE::LOGGER> log) override{
            this->logger_ = log;
            if (this->logger_)
                this->logger_->subscribe(this->ID, true);
        }


//        bool ref_ready{false};

        float signal_frequency{0.};
    private:
        std::vector<T> data_block;
        std::promise<void> reference_data_promise;
        std::shared_future<void> reference_data_future;

        unsigned processing_index{0};

        void fullfill_promise() {
            this->reference_data_promise = std::promise<void>();
            this->reference_data_future = std::shared_future<void>(this->reference_data_promise.get_future());
            this->reference_data_promise.set_value();
        }

        void process(bool print_ = false) override {
            if (print_) {
                printf("REF_%d ", this->ID);
                return;
            }
            unsigned i = 0;
            this->data_block.resize(this->block_size);
            for (unsigned idx = this->processing_index; idx < this->processing_index + this->block_size; idx++) {
                this->data_block[i] = this->data_Buffer.get(idx);
                i++;
            }
            this->processing_index = (this->processing_index + this->sliding_window_size) % this->capacity;

            if (this->filter) {
                std::vector<double> ref_data;
                ref_data.reserve(this->block_size);
                for (auto j = 0; j < this->block_size; j++)
                    ref_data[j] = this->data_block[j].data[0];
                double *const ref_d[] = {reinterpret_cast<double *const >(&ref_data[0])};
                this->filter->process(ref_data.size(), ref_d);
                for (auto j = 0; j < this->block_size; j++)
                    (double &) this->data_block[j].data[0] = ref_data[j];
            }


            //calculate frequency
            try{
                this->signal_frequency = ALGORITHMS::find_frequency(this->data_block, this->timebase);
            }
            catch (const std::exception& ex){
                std::cerr << "PROBLEM IN REF SIGNAL FREQUENCY CALCULATION: " << ex.what() << "\n";
            }
//            this->signal_frequency = 8;
            this->fullfill_promise();

        }


    };
//##################################################################################

//##################################################################################
    template<typename T=ENGINE::DataFrame<float, NM::COMPLEX>>
    class NormalChannel_Buffer : public ENGINE::BASE_CHANNEL_BUFFER<T> {
        /*a class representing one channel where data is entered, processed and logged*/
    public:
        NormalChannel_Buffer(int id, unsigned buffer_size, unsigned block_size_, unsigned slidingWindow_size,
                             double timebase_, unsigned results_buff_size, const std::string &coil_type,
                             std::map<int, RefChannel_Buffer<T> *>* refs=nullptr,
                             std::shared_ptr<ENGINE::LOGGER> logger=std::shared_ptr<ENGINE::LOGGER>(),
                             std::shared_ptr<task_system> tp=std::shared_ptr<task_system>()) :
                BASE_CHANNEL_BUFFER<T>(id, buffer_size, block_size_, slidingWindow_size, timebase_,
                                       coil_type, logger, tp), refChannels(refs) {
            if (this->logger_) {
                std::vector<int> refs_ids;
                refs_ids.reserve(this->refChannels->size());
                for (auto &ref: *this->refChannels) {
                    refs_ids.emplace_back(ref.first);
                }
                this->logger_->subscribe(id, false, refs_ids);
            }
            std::cout << this->ID << ";" << "IS NORMAL; " << this->sliding_window_size << ";" << this->block_size
                      << "\n";
            if (this->sliding_window_size > this->block_size)
                throw ENGINE::CHANNEL_ERROR(
                        std::string("NORMALCHANNEL: the size of the Siding Window is greater than the size of block!!!"
                                    + std::to_string(this->sliding_window_size) + ";"
                                    + std::to_string(this->block_size)));

            this->output_Buffers = std::unordered_map<int, simple_RingBuffer<T>>(this->refChannels->size());
            for (auto &ref: *this->refChannels)
                this->output_Buffers.emplace(ref.first, simple_RingBuffer<T>(results_buff_size));
            std::cout << "NOR: " << this->output_Buffers.size() << " Size: " << results_buff_size << "\n";
        }

        // move semantics
        NormalChannel_Buffer(NormalChannel_Buffer<T> &&ub)
                : BASE_CHANNEL_BUFFER<T>(std::move(ub)),
                  output_Buffers(std::move(ub.output_Buffers)), refChannels(ub.refChannels) {
            if (this->logger_) {
                std::vector<int> refs_ids;
                refs_ids.reserve(this->refChannels->size());
                for (auto &ref: *this->refChannels)
                    refs_ids.emplace_back(ref.first);
                this->logger_->subscribe(this->ID, false, refs_ids);
            }
            std::cout << this->ID << ";" << this->sliding_window_size << ";" << this->block_size << "\n";
            if (this->sliding_window_size > this->block_size)
                throw ENGINE::CHANNEL_ERROR(
                        std::string("NORMALCHANNEL: the size of the Siding Window is greater than the size of block!!!"
                                    + std::to_string(this->sliding_window_size) + ";"
                                    + std::to_string(this->block_size)));
        }


        std::string start_tracking() override {
            std::ostringstream streamObj;
            streamObj << "REGULAR Channel " << this->ID << ": buffer size: " << this->data_Buffer.size()
                      << " Results size: ";
            for (auto &buff: this->output_Buffers)
                streamObj << buff.second.size() << ";";
            streamObj << "\n";
            return streamObj.str();
        }

        void put_results(int ref_id, const T& data){
            if (this->logger_){
                this->logger_->log_res(this->ID, ref_id, data);
            }
            this->output_Buffers.at(ref_id).push_back(data);
        }
        ///channel id as key and ref_buffer pointer as value
        ///a null_ptr can be provided to deactivate running tasks in on the data in the background
        void set_ref_channels(std::map<int, RefChannel_Buffer<T> *>* refs_){
            refChannels = refs_;
        }

        void add_logger(std::shared_ptr<ENGINE::LOGGER> log) override{
            this->logger_ = log;
            if (this->logger_) {
                std::vector<int> refs_ids;
                refs_ids.reserve(this->refChannels->size());
                for (auto &ref: *this->refChannels)
                    refs_ids.emplace_back(ref.first);
                this->logger_->subscribe(this->ID, false, refs_ids);
            }
        }

        std::unordered_map<int, simple_RingBuffer<T>> output_Buffers;
        std::map<int, RefChannel_Buffer<T> *>* refChannels;

    private:
//        T df; //dummy data frame reused to avoid creating new objects
        unsigned processing_index{0};

        void process(bool print_ = false) override {
            if (print_) {
                printf("CH_%d ", this->ID);
                return;
            }
            if (refChannels) //only run if refChannels is not a null_ptr
                try {
                    this->job.ID = this->ID;
                    this->job.timebase = this->timebase;
                    unsigned i = 0;
                    this->job.data.resize(this->block_size);
                    for (unsigned idx = this->processing_index; idx < this->processing_index + this->block_size; idx++) {
                        this->job.data[i] = this->data_Buffer.get(idx); //buffer adjusts the index in the get function
                        i++;
                    }
                    if (this->filter) {
                        std::vector<double> ch_data;
                        ch_data.reserve(this->block_size);
                        for (auto j = 0; j < this->block_size; j++)
                            ch_data[j] = this->job.data[j].data[0];
                        double *const ch_d[] = {reinterpret_cast<double *const >(&ch_data[0])};
                        this->filter->process(ch_data.size(), ch_d);
                        for (auto j = 0; j < this->block_size; j++){
                            (double &) this->job.data[j].data[0] = ch_data[j];
                        }

                    }
                    this->processing_index = (this->processing_index + this->sliding_window_size) % this->capacity;
                    for (auto &ref: *this->refChannels) {
                        auto ref_id = ref.first; //necessary to avoid race conditions
                        //setup returning results callback for job
                        this->job.return_result = [ref_id, this](const T &d) {
                            this->put_results(ref_id, d);
                        };
    //                    printf("WAITING ON REF\n");
                        this->job.reference_data = ref.second->get_block(); //this is blocking
                        this->job.ref_frequency = ref.second->signal_frequency;
                        //update coil factors in the job struct
                        auto c = Coils_MAP.at(this->coil_type);
                        this->job.coil_factors.amplitude_correction = c->calculateAmplitudeCorrection(
                                ref.second->signal_frequency),
                        this->job.coil_factors.phase_correction = c->calculatePhaseCorrection(ref.second->signal_frequency),
                        this->job.coil_factors.vToB = c->get_vToB() * ref.second->signal_frequency;
                        //update ref coil factors
                        c = Coils_MAP.at(ref.second->coil_type);
                        this->job.ref_coil_factors.amplitude_correction = c->calculateAmplitudeCorrection(
                                ref.second->signal_frequency),
                        this->job.ref_coil_factors.phase_correction = c->calculatePhaseCorrection(
                                ref.second->signal_frequency),
                        this->job.ref_coil_factors.vToB = c->get_vToB() * ref.second->signal_frequency;

    //                    //run tasks

                        for (auto &f: this->tasks)
                            f(this->job);
                    }
                }
                catch (const std::exception &ex) {
    //                for (auto& ref: *this->refChannels){
    //                    ref.second.release();
    //                }
                    std::cerr << "ERROR IN REGULAR CHANNEL PROCESSING: " << ex.what() << "\n";
                }
        }


    };

}

#include <chrono>

unsigned long ts{0};

void test_basic_funcs() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    Config cg("C:\\Users\\User\\Desktop\\HZDR\\hzdr_tasler_data_acquisition\\tasler_config.json");
//    ENGINE::LOGGER Logger(cg);
    std::map<int, ENGINE::RefChannel_Buffer<ENGINE::DataFrame<float, NM::COMPLEX>> *> ref_map;
    std::map<int, ENGINE::NormalChannel_Buffer<ENGINE::DataFrame<float, NM::COMPLEX>> *> Buffer_map;
//    ref_map.reserve(16);
//    Buffer_map.reserve(16);
    auto tp = std::make_shared<task_system>(1);

    auto signal_ = NM::sin_(1, 1, 8, 0, 200000e-9);
    printf("size: %d\n", signal_.size());
    for (unsigned i = 0; i < 4; i++) {
        ref_map.emplace(i, new ENGINE::RefChannel_Buffer<ENGINE::DataFrame<float, NM::COMPLEX>>(i, 40960,
                                                                                                signal_.size() / 4,
                                                                                                signal_.size() / 8,
                                                                                                1e-2,
                                                                                                "2x160K_high_impedance",
                                                                                                nullptr,
                                                                                                tp));
    }

    for (unsigned i = 4; i < 32; i++) {
        Buffer_map.emplace(i, new ENGINE::NormalChannel_Buffer<ENGINE::DataFrame<float, NM::COMPLEX>>(i, 40960,
                                                                                                      signal_.size() /
                                                                                                      4,
                                                                                                      signal_.size() /
                                                                                                      8, 1e-2, 1e3,
                                                                                                      "2x160K_high_impedance",
                                                                                                      &ref_map,
                                                                                                      nullptr,
                                                                                                      tp));
    }

    ref_map.begin()->second->set_operation_mode(ENGINE::SHARED);

    testing_timer.setInterval([&]{
        for (auto& ref: ref_map)
            std::cout << ref.second->start_tracking();
        for(auto& ch: Buffer_map)
            std::cout << ch.second->start_tracking();
    }, (int) 5*1e6);

    for (auto &b: ref_map) {
        b.second->add_filter(create_default_filter());
        b.second->run();
    }

    for (auto &c: Buffer_map) {
        c.second->add_filter(create_default_filter());

        c.second->add_task(ENGINE::clean_reference_signal);
        c.second->add_task(ENGINE::demodulate);
        c.second->run();
    }

    testing_timer.setInterval([&]{
            try{
//                PerfTimer pertim("Writing data to channels ");
                for (unsigned i=0; i<std::rint(signal_.size()/8); i++) { //std::rint(signal_.size()/8)
                    for (auto &b: ref_map) {
//                        std::cout << b.first << ":" << b.second.ID << "\n";
                        b.second->put((float) ts / 100,
                                     1000 * signal_[ts % signal_.size()], false);//1000*signal_[ts%signal_.size()]
                    }

                    for (auto &b: Buffer_map){
//                        std::cout << b.first << ":" << b.second.ID << "\n";
                        b.second->put((float) ts / 100,
                                      1000 * signal_[ts % signal_.size()], false);//1000*signal_[ts%signal_.size()]
                    }
                    ts++;
                }
//                    std::this_thread::sleep_for(std::chrono::microseconds((int)1e6));

            } catch(const std::exception& ex){
                std::cerr << "ERROR IN Generator: " << ex.what() << "\n";
            }

    }, (int)1);
    std::cin.get();
    for (auto& b: ref_map){
        delete b.second;
    }
//
    for (auto& b: Buffer_map){
        delete b.second;
    }

}

#endif //HZDR_DATA_ACQUISITION_CHANNELDATA_H
