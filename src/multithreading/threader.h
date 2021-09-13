#ifndef THREADER_H
#define THREADER_H

#include <vector>
#include <queue>
#include <chrono>
#include <thread>

#include "thread_render_task.h"
#include "multithreading/semaphore.h"

template <typename T>
class Threader {
    std::vector<T> tasks;

    std::vector<std::thread*> threads;
    std::vector<Semaphore> sems_start;
    std::vector<Semaphore> sems_stop;

    void (*func)(T&);

    bool running;

    void thread_main_loop(int thread_id) {  
        while (running) {
            sems_start[thread_id].wait();
            func(tasks[thread_id]);
            sems_stop[thread_id].post();
        }
    }

    static void call_thread_main_loop_method(void *that, int thread_id) {
        ((Threader<T>*) that)->thread_main_loop(thread_id);
    }

public:
    Threader():
    tasks(),
    threads(),
    func(nullptr),
    running(true)
    {}

    Threader(size_t thread_count, void (*func)(T&)):
    tasks(),
    threads(),
    func(func),
    running(true)
    {
        for (size_t i = 0; i < thread_count; ++i) {
            threads.push_back(new std::thread(Threader::call_thread_main_loop_method, this, i));
            sems_start.push_back({0});
            sems_start.push_back({0});
        }
    }

    inline void join() {
        running = false;
        for (size_t i = 0; i < threads.size(); ++i) {
            threads[i]->join();
        }
    }

    inline size_t get_threads_cnt() const {
        return threads.size();
    }

    inline void add_task(const T &task) {
        if (tasks.size() < get_threads_cnt())
            tasks.push_back(task);
        else {
            printf("[ERR]<threader> adding more tasks will leave them undone, use set_tasks instead\n");
        }
    }

    inline void set_task(size_t i, const T &task) {
        if (i > tasks.size()) {
            printf("[ERR]<threader> task index %lu is far too big (cur: %lu) \n", i, tasks.size());
            return;
        }

        tasks[i] = task;
    }

    void perform() {
        size_t threads_cnt = threads.size();
        size_t tasks_cnt = tasks.size();
        int threads_to_use = tasks_cnt < threads_cnt ? tasks_cnt : threads_cnt;

        for (int i = 0; i < threads_to_use; ++i) {
            sems_start[i].post();
        }

        for (int i = 0; i < threads_to_use; ++i) {
            sems_stop[i].wait();
        }
    }

};


#endif // THREADER_H