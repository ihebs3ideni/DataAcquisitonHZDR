//
// Created by iheb on 13.05.2022.
//

#ifndef HZDR_DATA_ACQUISITION_BASEPROCESS_H
#define HZDR_DATA_ACQUISITION_BASEPROCESS_H

#include <vector>
#include <memory>
#include <thread>

class Process{

protected:
    bool shut_down_{false};
public:
    Process(unsigned process_id): ID(process_id){}
    Process(Process&& p): ID(std::move(p.ID)){}

    bool running{false};

    virtual void run();

    virtual void stop();

    bool shut_down() const{
        return shut_down_;
    }
    int ID;
    std::vector<std::shared_ptr<std::thread>> task_thread;
};

void Process::run() {
    ///responsible for creating and starting the thread within which this process will run
}

void Process::stop() {
    ///responsible for cleaning up before and terminating the process e.g: queue.snapOut(), shut_down_=true ...
}


#endif //HZDR_DATA_ACQUISITION_BASEPROCESS_H
