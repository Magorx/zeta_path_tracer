#pragma once


#include "worker.h"

#include <string>
#include <thread>


namespace jobber::worker {

template <TaskConcept TaskT>
class LocalT : public WorkerT<TaskT> {
private:
    using ResultT = typename TaskT::ResultT;

private:
    size_t id_;
    static size_t MAX_ID;

    std::string name_;

private:
    std::thread thread_;

    TaskT current_task_ = TaskT::empty();
    ResultT current_result_;

    enum StateT {
        idle,
        task_ready,
        executing_task,
        result_ready
    };
    std::atomic<StateT> state_;

    std::atomic<bool> is_online = true;

private:
    void routine() {
        while (is_online) {
            StateT awaited_state = StateT::task_ready;
            auto log = logger.debug(name_);

            while (is_online && !state_.compare_exchange_strong(awaited_state, StateT::executing_task)) {
                log << " waiting for task";
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                awaited_state = StateT::task_ready;
            }

            log << "task exec";
            current_result_ = current_task_.run();

            if constexpr (true) {
                log << "task done | result = " << current_result_;
            } else {
                log << "task done";
            }

            state_.store(StateT::result_ready);
        }
    }

public:
    LocalT()
        : id_(MAX_ID++)
        , name_("w" + std::to_string(id_))
        , state_(StateT::idle)
    {
        thread_ = std::thread(&LocalT::routine, this);
    }

    ~LocalT() {
        is_online = false;
        thread_.join();
    }

    virtual void set_task(TaskT &&task) override {
        if (state_ != StateT::idle) {
            assert(false && "worker at not idle state is being given task");
        }

        current_task_ = std::move(task);
        state_.store(StateT::task_ready);
    }

    virtual bool is_busy() const override {
        return state_ != StateT::idle && state_ != StateT::result_ready;
    }

    virtual ResultT get_result() override {
        StateT awaited_state = StateT::result_ready;
        if (!state_.compare_exchange_strong(awaited_state, StateT::idle)) {
            assert(false && "worker at not result_ready state is being asked for result");
        }

        return std::move(current_result_);
    }
};

template <TaskConcept TaskT>
size_t LocalT<TaskT>::MAX_ID = 0;


} // namespace jobber::worker