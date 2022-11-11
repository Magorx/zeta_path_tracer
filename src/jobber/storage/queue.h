#pragma once


#include "storage.h"

#include <queue>


namespace jobber::storage {

template <jobber::TaskConcept TaskT>
class QueueT {
private:
    std::queue<TaskT> tasks;
public:
    void put(std::vector<TaskT> &&tasks) {
        for (auto &&task : tasks) {
            this->tasks.push(std::move(task));
        }
    }

    std::vector<TaskT> take(size_t n_tasks) {
        std::vector<TaskT> result;
        for (size_t i = 0; i < n_tasks; ++i) {
            if (tasks.empty()) {
                break;
            }
            result.push_back(std::move(tasks.front()));
            tasks.pop();
        }
        return result;
    }

    size_t size() {
        return tasks.size();
    }

    ComplexityT complexity() {
        ComplexityT result = 0;
        for (const auto &task : tasks) {
            result += task.complexity();
        }
        return result;
    }
};


} // namespace jobber::storage
