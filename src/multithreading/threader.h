#ifndef THREADER_H
#define THREADER_H

#include <vector>
#include <queue>
#include <chrono>
#include <thread>

#include "thread_render_task.h"
#include "multithreading/semaphore.h"

#include <pthread.h>

template <typename T>
class Threader {
    std::vector<T> tasks;
    

    std::vector<pthread_t> threads;
    std::vector<Semaphore> sems_start;
    std::vector<Semaphore> sems_stop;

    void (*func)(T&);

    bool running;

    int max_t;

    void thread_main_loop(int thread_id) {

            sems_start[thread_id].post();
        while (running) {
            printf("? thr %d awaits | value = %d\n", thread_id, sems_start[thread_id].get_value());
            sems_start[thread_id].wait();
            printf("--- thr %d got\n", thread_id);

            // func(tasks[thread_id]);
            
            // sems_stop[thread_id].post();
            printf("+ thr %d done\n", thread_id);
        }
    }

    static void *call_thread_main_loop_method(void *that) {
        ((Threader<T>*) that)->thread_main_loop(((Threader<T>*) that)->max_t++);
        return nullptr;
    }

public:
    Threader():
    tasks(),
    threads(),
    func(nullptr),
    running(true),
    max_t(0)
    {}

    Threader(size_t thread_count, void (*func)(T&)):
    tasks(),
    threads(thread_count),
    func(func),
    running(true),
    max_t(0)
    {
        for (size_t i = 0; i < thread_count; ++i) {
            // threads.push_back(new std::thread(Threader::call_thread_main_loop_method, this, i));
            pthread_create(&threads[i], nullptr, call_thread_main_loop_method, this);
            sems_start.push_back({0});
            sems_stop.push_back({0});
        }
    }

    inline void join() {
        running = false;
        for (size_t i = 0; i < threads.size(); ++i) {
            // threads[i]->join();
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

        // sems_start[0].post();
        // sems_start[1].post();
        // sems_start[2].post();
        // sems_start[3].post();

        for (int i = 0; i < threads_to_use; ++i) {
            printf("le go %d\n", i);
            sems_start[i].post();
            printf("%d value = %d\n", i, sems_start[i].get_value());
        }

        for (int i = 0; i < threads_to_use; ++i) {
            sems_stop[i].wait();
        }
    }

};


#endif // THREADER_H