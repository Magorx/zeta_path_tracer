#pragma once


#include "splitter.h"

#include <list>
#include <cmath>


namespace jobber::splitter {

template <jobber::TaskConcept TaskT>
class LinearBisecting {
private:
    size_t complexity_threshhold;
public:
    LinearBisecting(size_t complexity_threshhold)
        : complexity_threshhold(complexity_threshhold)
    {}

    std::vector<TaskT> split(TaskT &&task) const {
        std::list<TaskT> tasks = {std::move(task)};
        while (true) {
            auto it = std::find_if(tasks.begin(), tasks.end(), [this](const auto &task) {
                return task.complexity() > complexity_threshhold;
            });
            if (it == tasks.end()) {
                break;
            }

            auto new_tasks = it->split(2);

            tasks.erase(it);
            tasks.insert(tasks.end(), new_tasks.begin(), new_tasks.end());
        }

        return std::vector<TaskT>(tasks.begin(), tasks.end());
    }
};


template <jobber::TaskConcept TaskT>
class LinearGreedy {
private:
    size_t complexity_threshhold;
public:
    LinearGreedy(size_t complexity_threshhold) 
        : complexity_threshhold(complexity_threshhold)
    {}

    std::vector<TaskT> split(TaskT &&task) const {
        std::list<TaskT> tasks = {std::move(task)};
        while (true) {
            auto it = std::find_if(tasks.begin(), tasks.end(), [this](const auto &task) {
                return task.complexity() > complexity_threshhold;
            });
            if (it == tasks.end()) {
                break;
            }

            auto complexity = it->complexity();
            auto n_tasks = std::ceil(complexity / complexity_threshhold);

            auto new_tasks = it->split(n_tasks);
            tasks.erase(it);
            tasks.insert(tasks.end(), new_tasks.begin(), new_tasks.end());
        }

        return std::vector<TaskT>(tasks.begin(), tasks.end());
    }
};

} // namespace jobber::splitter
