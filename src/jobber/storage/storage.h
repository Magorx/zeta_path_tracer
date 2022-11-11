#pragma once


#include <jobber/task/task.h>

#include <concepts>
#include <vector>

namespace jobber {

template<typename StorageT, typename TaskT>
concept StorageConcept = requires(
    StorageT storage,
    std::vector<TaskT> &&tasks,
    size_t n_tasks
) {
    { storage.put(std::move(tasks))    };
    { storage.take(n_tasks)            } -> std::convertible_to<std::vector<TaskT>>;

    { storage.size()                   } -> std::convertible_to<size_t>;
    { storage.complexity()             } -> std::convertible_to<ComplexityT>;
};

} // namespace jobber
