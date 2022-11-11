#pragma once


#include <jobber/task/task.h>

#include <concepts>
#include <vector>

namespace jobber {

template<typename SplitterT, typename TaskT>
concept SplitterConcept = requires(
    SplitterT splitter,
    TaskT &&task
) {
    { splitter.split(std::move(task)) } -> std::convertible_to<std::vector<TaskT>>;
};

} // namespace jobber
