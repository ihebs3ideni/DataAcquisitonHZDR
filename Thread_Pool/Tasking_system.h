//
// Created by iheb on 11/3/2020.
//

#ifndef HZDR_TASLER_TP_THREAD_POOL_H
#define HZDR_TASLER_TP_THREAD_POOL_H


#include "../DataStructures/notification_queue.h"
#include <vector>
//using namespace std;

class task_system {
    unsigned _count{std::thread::hardware_concurrency()};
    std::vector<std::thread> _threads;
    std::vector<notification_queue> _q{_count};
    std::atomic<unsigned> _index{0};
    int K = 1;

    void run(unsigned i) {
        while (true) {
            std::function<void()> f;
            for (unsigned n = 0; n != _count; ++n) {
                if (_q[(i + n) % _count].try_pop(f)) break;
            }
            if (!f && !_q[i].pop(f)) break;
            f();
        }
    }

public:
    task_system() {
        for (unsigned n = 0; n != _count; ++n) {
            _threads.emplace_back(std::thread([&, n]{ run(n); }));
        }
    }

    task_system(unsigned int count) {
        if (count != 0) _count = count;
        for (unsigned n = 0; n != _count; ++n) {
            _threads.emplace_back([&, n]{ run(n); });
        }
        std::cout << _count << " Threads created in the pool.\n";
    }

    ~task_system() {
        for (auto& e : _q) e.done();
        for (auto& e : _threads) e.join();
    }

    template <typename F>
    void async_(F&& f) {
        auto i = _index++;
        for (unsigned n = 0; n != _count * K; ++n) {
            if (_q[(i + n) % _count].try_push(std::forward<F>(f))) return;
        }
        _q[i % _count].push(std::forward<F>(f));
    }
};

void test_threadPool(){
    task_system tp(2);
    tp.async_([]{std::cout << "TASK 1 DONE\n";});
    tp.async_([]{std::cout << "TASK 2 DONE\n";});
    tp.async_([]{std::cout << "TASK 3 DONE\n";});
    tp.async_([]{std::cout << "TASK 4 DONE\n";});
    std::cin.get();
}

#endif //HZDR_TASLER_TP_THREAD_POOL_H
