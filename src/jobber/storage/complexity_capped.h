#pragma once


#include <jobber/splitter/splitter.h>

#include "queue.h"


namespace jobber::storage {

template <TaskConcept TaskT, SplitterConcept<TaskT> SplitterT>
class ComplexityCappedT : public QueueT<TaskT> {
private:
    SplitterT splitter;
public:
    ComplexityCappedT(SplitterT splitter)
        : splitter(std::move(splitter))
    {}

    void put(std::vector<TaskT> &&tasks) {
        for (auto &&task : tasks) {
            auto subtasks = splitter.split(std::move(task));
            QueueT<TaskT>::put(std::move(subtasks));
        }
    }

    std::vector<TaskT> take(size_t n_tasks) {
        return QueueT<TaskT>::take(n_tasks);
    }
};

} // namespace jobber::storage
