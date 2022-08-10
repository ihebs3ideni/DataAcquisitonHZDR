//
// Created by iheb on 07.05.2022.
//

#ifndef HZDR_DATA_ACQUISITION_TS_QUEUE_H
#define HZDR_DATA_ACQUISITION_TS_QUEUE_H


#include <algorithm>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <condition_variable>
#include <chrono>

using namespace std;

class non_empty_queue : public std::exception {
    std::string what_;
public:
    explicit non_empty_queue(std::string msg) { what_ = std::move(msg); }
    const char* what() const noexcept override  { return what_.c_str(); }
};

template<typename T>
class ThreadsafeQueue {
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    condition_variable _ready;
    bool snapout{false};

    // Moved out of public interface to prevent races between this
    // and pop().


public:
    ThreadsafeQueue() = default;
    ThreadsafeQueue(const ThreadsafeQueue<T> &) = delete ;
    ThreadsafeQueue& operator=(const ThreadsafeQueue<T> &) = delete ;

    ThreadsafeQueue(ThreadsafeQueue<T>&& other) noexcept(false) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!empty()) {
            throw non_empty_queue("Moving into a non-empty queue"s);
        }
        queue_ = std::move(other.queue_);
    }

    virtual ~ThreadsafeQueue() noexcept(false) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!empty()) {
            throw non_empty_queue("Destroying a non-empty queue"s);
        }
    }

    unsigned long size() const {
        std::unique_lock<std::mutex> lock(mutex_);
        return queue_.size();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return {};
        }
        T tmp = queue_.front();
        queue_.pop();
        return tmp;
    }

    std::optional<T> pop_blocking(){
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty() & !snapout) _ready.wait(lock);
        snapout = false;
        if (queue_.empty()) return {}; //in case snapOut is called
        T tmp = queue_.front();
//        x = move(queue_.front());
        queue_.pop();
        return tmp;
    }

    void push(const T &item) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push(item);
        }
        _ready.notify_all();
    }

    bool empty() const {
//        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    void snapOut(){
        snapout= true;
        _ready.notify_all();
    }
};


void test_TSQ(){
    ThreadsafeQueue<int> RAW_INPUT;
    std::thread t([&]() {
        int i = 0;
        std::string data ;
        while(1){

            getline(std::cin, data);
            std::cout << "data: " << data << "\n";
//            RAW_INPUT.snapOut();
            RAW_INPUT.push(i*69.420);
            std::this_thread::sleep_for(std::chrono::microseconds((int)1e6));
            if (i++ > 100)
                return;

        }

    });

    std::thread t2([&]() {
        int i = 0;
        while(1){
            auto data = RAW_INPUT.pop_blocking();
            std::cout << "unblocked" << "\n";
            if (i++ > 100)
                return;
        }
    });
    t.join();
    t2.join();
}


#endif //HZDR_DATA_ACQUISITION_TS_QUEUE_H
