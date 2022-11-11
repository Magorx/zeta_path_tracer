#pragma once


#include "pipeline.h"


namespace jobber::pipeline {

template <
    TaskConcept TaskT,
    StorageConcept<TaskT> StorageT
>
class ThreadLocalT {
private:
    using ResultT = typename TaskT::ResultT;

private:
    StorageT storage;

public:
    ThreadLocalT(StorageT storage)
        : storage(std::move(storage))
    {}

    void put(TaskT &&task) {
        storage.put(std::move(std::vector{task}));
    }

    ResultT await() {
        auto tasks = storage.take(storage.size());
        std::vector<ResultT> results;

        for (auto &&task : tasks) {
            results.push_back(task.run());
        }

        return TaskT::reduce(std::move(results));
    }
};

} // namespace jobber::pipeline
