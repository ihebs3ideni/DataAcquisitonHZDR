//
// Created by IHEB on 30/06/2022.
//

#ifndef HZDR_DATA_ACQUISITION_EVENTS_H
#define HZDR_DATA_ACQUISITION_EVENTS_H
#include <mutex>

namespace EVENTSPACE{
    struct event{
    private:
        std::mutex mtx;
        std::condition_variable cv;
        std::atomic<bool> ready{false};
        unsigned _count{1};
        unsigned n_users{1};
    public:

        bool notify(){
            this->ready = true;
            this->cv.notify_all();
            this->_count = this->n_users; //reset count to number of users
            return true;
        }

        void wait(){
            std::unique_lock<std::mutex> lck(this->mtx); // aquire lock
            this->cv.wait(lck, [this]{ return this->ready.load();}); //wait for event to trigger
        }

        void release(){
            this->ready = false;
        }

        void set_users(unsigned nUsers){
            _count = nUsers;
            n_users = nUsers;
        }

        void partial_release(){
            ///uses count for multiple users to release the event gradually
            if(_count == 1){
                this->release();
                return;
            }
            _count-=1;
        }
};

}


#include <thread>
void test_event(){
    const unsigned nUsers = 32;
    EVENTSPACE::event e;
    e.set_users(nUsers);

    auto t_ = std::thread([&](){
        for (unsigned i=0; i<nUsers; i++){
            e.wait();
            std::cout << "t_"<< i <<" event captured\n";
            e.partial_release();
        }
    });

    auto ts = std::thread([&]() {
        std::cin.get();
        e.notify();
        std::cout << "ts triggered the event\n";
    });
    ts.detach();
    t_.join();
}

#endif //HZDR_DATA_ACQUISITION_EVENTS_H
