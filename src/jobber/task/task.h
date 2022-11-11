#pragma once


#include <utils/logger.h>

#include <concepts>
#include <cstddef>
#include <vector>

namespace jobber {

using ComplexityT = size_t;

template<typename TaskT>
concept TaskConcept = requires(
    TaskT task,
    size_t n_tasks,
    const typename TaskT::ComputeContextUpdateT &compute_context_update,
    const std::vector<typename TaskT::ResultT> &results
) {
    { task.run()                        } -> std::convertible_to<typename TaskT::ResultT>;
    { task.complexity()                 } -> std::convertible_to<ComplexityT>;

    { task.split(n_tasks)               } -> std::convertible_to<std::vector<TaskT>>;
    { TaskT::reduce(std::move(results)) } -> std::convertible_to<typename TaskT::ResultT>;

    { task.update_compute_context(compute_context_update) };

    { TaskT::empty()                    } -> std::convertible_to<TaskT>;
    { task.is_empty()                   } -> std::convertible_to<bool>;
};

} // namespace jobber
