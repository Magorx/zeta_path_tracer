#pragma once


#include "worker_driven.h"
#include <jobber/worker/local.h>


namespace jobber::pipeline {

template <
    TaskConcept TaskT,
    StorageConcept<TaskT> StorageT
>
class WorkerThreadedT : public WorkerDrivenT<TaskT, StorageT> {
private:
    using ResultT = typename TaskT::ResultT;
    using WorkerT = worker::LocalT<TaskT>;

private:
    std::vector<WorkerT> workers_;

public:
    WorkerThreadedT(StorageT storage, size_t workers)
        : WorkerDrivenT<TaskT, StorageT>(storage)
        , workers_(workers)
    {
        for (auto &worker : workers_) {
            WorkerDrivenT<TaskT, StorageT>::add_worker(worker);
        }
    }

    ~WorkerThreadedT() {
    }

};

} // namespace jobber::pipeline
