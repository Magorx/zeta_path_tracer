#pragma once


#include "pipeline.h"

#include <jobber/worker/worker.h>

#include <utils/progress_bar.h>

#include <asio.hpp>

#include <vector>
#include <queue>
#include <mutex>


namespace jobber::pipeline {

template <
    TaskConcept TaskT,
    StorageConcept<TaskT> StorageT
>
class WorkerDrivenT {
private:
    using ResultT = typename TaskT::ResultT;
    using WorkerT = worker::WorkerT<TaskT>;

    using GivenTaskT = TaskT;

private:
    struct StatusT {
        std::atomic<bool> is_online_;
        std::atomic<bool> stopped_externaly_;
        std::atomic<bool> tasks_available_;
    };
    

private:
    StatusT status_ = {
        true,
        false,
        true
    };

    StorageT storage_;
    std::mutex storage_mutex_;

    std::vector<ResultT> results_;
    std::mutex results_mutex_;

    std::atomic<int> await_in_progress = 0;

    asio::io_context io_;
    std::thread io_runner_;

private:
    bool is_online() {
        return status_.is_online_;
    }

    bool set_online(bool is_online) {
        status_.is_online_ = is_online;
        return is_online;
    }

    bool has_to_await() {
        return !status_.stopped_externaly_ && (
            false
            || await_in_progress
            || status_.tasks_available_
            || storage_.size() > 0
        );
    }

private:
    asio::awaitable<void> small_sleep() {
        asio::steady_timer timer(io_, std::chrono::milliseconds(1));
        co_await timer.async_wait(asio::use_awaitable);
        co_return;
    }

private:
    asio::awaitable<GivenTaskT> get_task() {
        while (status_.tasks_available_) {
            storage_mutex_.lock();

            if (storage_.size() > 0) {
                auto tasks = storage_.take(1);
                storage_mutex_.unlock();
                co_return std::move(tasks[0]);
            } else {
                status_.tasks_available_ = false;
            }

            storage_mutex_.unlock();

            co_await small_sleep();
        }

        co_return TaskT::empty();
    }

    void push_result(ResultT &&result) {
        std::lock_guard lock(results_mutex_);
        results_.push_back(std::move(result));
    }

    asio::awaitable<void> worker_routine(WorkerT &worker) {
        logger.trace << "worker routine started";

        while (is_online()) {
            await_in_progress++;

            auto task = co_await get_task();
            if (task.is_empty()) {
                await_in_progress--;
                co_await small_sleep();
            } else {
                logger.trace << "found task";

                worker.set_task(std::move(task));

                while (worker.is_busy()) {
                    co_await small_sleep();
                }

                push_result(std::move(worker.get_result()));

                await_in_progress--;
            }            
        }

        logger.trace << "worker routine finished";
    }

private:
    void run_io() {
        asio::executor_work_guard<asio::io_context::executor_type> work_guard(io_.get_executor());

        io_.run();
    }

    void io_runner_routine() {
    }

public:
    WorkerDrivenT(StorageT storage)
        : storage_(std::move(storage))
        , io_runner_(&WorkerDrivenT::run_io, this)
    {}

    ~WorkerDrivenT() {
        set_online(false);
        io_.stop();
        io_runner_.join();
    }

    void add_worker(WorkerT &worker) {
        asio::co_spawn(io_, [this, &worker]() -> asio::awaitable<void> {
            logger.debug << "spawned worker";
            co_await worker_routine(worker);

            co_return;
        }, asio::detached);
    }

    void put(TaskT &&task) {
        std::lock_guard lock(storage_mutex_);
        storage_.put(std::move(std::vector{task}));
    }

    ResultT await() {
        status_.tasks_available_ = true;

        kctf::ProgressBar progress_bar(100, "Awaiting results");

        size_t tasks_to_complete = storage_.size();

        while (has_to_await()) {
            logger.trace << "tasks: " << storage_.size() << ", await_in_progress: " << await_in_progress;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            progress_bar.update((double) (tasks_to_complete - storage_.size()) / tasks_to_complete);
        }

        logger.debug << "finished awaiting";

        return TaskT::reduce(std::move(results_));
    }

public:
    void stop() {
        status_.stopped_externaly_ = true;
    }

    void restart() {
        status_.stopped_externaly_ = false;
    }
};

} // namespace jobber::pipeline
