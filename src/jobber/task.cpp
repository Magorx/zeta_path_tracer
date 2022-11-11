#include "task.h"


namespace zephyr {

TaskT::ComputeContextT TaskT::compute_context {
    {
        nullptr,
        nullptr
    },
    zephyr::tracer::config::FullT()
};

} // namespace zephyr
