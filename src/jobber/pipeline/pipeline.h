#pragma once


#include <jobber/task/task.h>
#include <jobber/storage/storage.h>
#include <jobber/splitter/splitter.h>


namespace jobber {

template<typename PipelineT, typename TaskT>
concept PipelineConcept = requires(
    PipelineT pipeline,
    TaskT task
) {
    { pipeline.put(std::move(task)) };
    { pipeline.await()              } -> std::convertible_to<typename TaskT::ResultT>;
};

};
