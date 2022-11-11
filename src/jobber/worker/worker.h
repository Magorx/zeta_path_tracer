#pragma once


#include <jobber/task/task.h>


namespace jobber::worker {

template <TaskConcept TaskT>
class WorkerT {
protected:
    using ResultT = typename TaskT::ResultT;

public:
    virtual void set_task(TaskT &&task) = 0;
    virtual bool is_busy() const = 0;
    virtual ResultT get_result() = 0;

    virtual ~WorkerT() = default;
};

} // namespace jobber::worker
