//
// Created by iheb on 11/3/2020.
//

#ifndef HZDR_TASLER_TP_NOTIFICATION_QUEUE_H
#define HZDR_TASLER_TP_NOTIFICATION_QUEUE_H


#include <thread>
#include <mutex>
#include <functional>
#include <deque>
#include <iostream>
//using namespace std;
using lock_t = std::unique_lock<std::mutex>;

class notification_queue {

    bool _done{false};
    std::mutex _mutex;
    std::condition_variable _ready;
public:
    std::deque <std::function<void()>> _q;
    void clear(){
        _q.clear();
    }

    bool pop(std::function<void()>& x) {
        lock_t lock{_mutex};
        while (_q.empty() && !_done) _ready.wait(lock);
        if (_q.empty()) return false;
        x = move(_q.front());
        _q.pop_front();
        return true;
    }

    template<typename F>
    void push(F&& f) {
        {
            lock_t lock{_mutex};
            _q.emplace_back(std::forward<F>(f));
        }
        _ready.notify_one();
    }

    bool try_pop(std::function<void()> &x) {
        lock_t lock{_mutex,std::try_to_lock};
        if (!lock || _q.empty()) return false;
        x = move(_q.front());
        _q.pop_front();
        return true;
    }

    template<typename F>
    bool try_push(F &&f) {
        {
            lock_t lock{_mutex, std::try_to_lock};
            if (!lock) return false;
            _q.emplace_back(std::forward<F>(f));
        }
        _ready.notify_one();
        return true;
    }

    void done() {
        {
            std::unique_lock<std::mutex> lock{_mutex};
            _done = true;
        }
        _ready.notify_all();
    }
};

#endif //HZDR_TASLER_TP_NOTIFICATION_QUEUE_H
