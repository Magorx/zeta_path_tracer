#ifndef THREADER_H
#define THREADER_H

#include <vector>
#include <queue>
#include <chrono>
#include <thread>

#include "thread_render_task.h"


template <typename T>
class Threader {
    std::queue<T> task_queue;
    std::vector<std::thread*> threads;
    void (*func)(T&);

    int cur_threads_in_work;

    bool lock;

    bool running;
    bool joining;

    int max_thread_id;

    void thread_main_loop(int thread_id) {
        while (running && !(joining && !task_queue.size())) {
            while (lock) {
                std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10));
                continue;
            }

            if (task_queue.size()) {
                lock = !lock;
                if (!lock) {
                    continue;
                }

                ++cur_threads_in_work;

                T task = task_queue.front();
                task_queue.pop();

                lock = false;
                // printf("[thr] id [%d] strt\n", thread_id);
                func(task);
                // printf("[thr] id [%d] done\n", thread_id);

                --cur_threads_in_work;
            }
        }
    }

    static void call_thread_main_loop_method(void *that) {
        ((Threader<T>*) that)->thread_main_loop(++((Threader<T>*) that)->max_thread_id);
    }

public:
    Threader(size_t thread_count, void (*func)(T&)):
    task_queue(),
    threads(),
    cur_threads_in_work(0),
    func(func),

    lock(false),

    running(true),
    joining(false),

    max_thread_id(0)
    {
        for (size_t i = 0; i < thread_count; ++i) {
            threads.push_back(new std::thread(Threader::call_thread_main_loop_method, this));
        }
    }

    inline void add_task(T &task) {
        task_queue.push(task);
    }

    void join() {
        joining = true;

        for (size_t i = 0; i < threads.size(); ++i) {
            threads[i]->join();
        }
    }

    inline void do_lock() {
        lock = true;
    }

    inline void do_unlock() {
        lock = false;
    }

    inline int get_threads_cnt() const {
        return threads.size();
    }

    inline void wait() const {
        while (task_queue.size() || cur_threads_in_work) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

};


#endif // THREADER_H