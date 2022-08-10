//
// Created by iheb on 09.05.2022.
//

#ifndef HZDR_DATA_ACQUISITION_TIMER_H
#define HZDR_DATA_ACQUISITION_TIMER_H
#include <iomanip>
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>




class Timer {
    bool clear = false;
    mutable std::mutex mutex_;

public:
   // template<typename F, typename argtype=char>
    void setTimeout(std::function<void()> function, int delay_mrs) {
        //delay in microsecond
        {
            std::lock_guard<std::mutex> lock(mutex_);
            this->clear = false;
        }
        auto func = move(function);
        std::thread t([=, this]() {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::microseconds(delay_mrs));
            if(this->clear) return;
            func();
        });
        t.detach();
    }

//    template<typename F>
    void setInterval(std::function<void()> function, int interval_mrs) {
        //interval in microsecond
        {
            std::lock_guard<std::mutex> lock(mutex_);
            this->clear = false;
        }
        auto func = move(function);
        std::thread t([func, this, interval_mrs]() {
            while(true) {
                if(this->clear) return;
                std::this_thread::sleep_for(std::chrono::microseconds(interval_mrs));
                if(this->clear) return;
                func();
            }
        });
        t.detach();
    }

    void stop() {
        std::lock_guard<std::mutex> lock(mutex_);
        this->clear = true;
    }
};

#include <ctime>
#include <chrono>
void timer_callback(){
    std::time_t calltime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "timer_called at : " << std::ctime(&calltime) << "\n";
}

void test_timer(){
    Timer t;
    t.setInterval(timer_callback, (int)3e+3);

}

//******************************************************************************
// Struct created to time the performance of functions, to use it an instance of this struct
// should simply be created at the start of the function.
//******************************************************************************
struct PerfTimer{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> duration;
    std::string id;
    std::ostringstream streamObj;
public:
    explicit PerfTimer(std::string id_in): id(id_in)
    {
        start = std::chrono::high_resolution_clock::now();
    }
    ~PerfTimer() {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;

        streamObj << id << " took " << duration.count()*1000.00f << "ms to run\n";
        std::cout << streamObj.str();
    }
};




std::string get_timestamp(){
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%d_%m_%Y_%H_%M_%S");
    oss << '_' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
//        auto t = std::time(nullptr);
//        auto tm = *std::localtime(&t);
//
//        std::ostringstream oss;
//        oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S-%MS");
//        return oss.str();
}

std::string get_date(){
    using namespace std::chrono;
    auto now = system_clock::now();
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%d.%m.%Y");
    return oss.str();
}

std::string get_time(bool ms = false){
    using namespace std::chrono;
    auto now = system_clock::now();
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%H:%M:%S");
    if(ms){
        auto ms_ = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        oss << ':' << std::setfill('0') << std::setw(3) << ms_.count();
    }
    return oss.str();
}

std::chrono::time_point<std::chrono::high_resolution_clock> get_time_point(){
    return std::chrono::high_resolution_clock::now();
}

//in ms
float get_time_difference(std::chrono::time_point<std::chrono::high_resolution_clock>& begin,
                          std::chrono::time_point<std::chrono::high_resolution_clock>& end){
    std::chrono::duration<float> duration = end-begin;
    return duration.count()*1000.00f;
}



#endif //HZDR_DATA_ACQUISITION_TIMER_H
