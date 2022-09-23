#ifndef MT_PATH_TRACER
#define MT_PATH_TRACER

#include "threader.h"
#include "thread_render_task.h"

namespace zephyr::threading {

void render_threaded(ThreadRenderTaskT &task);

} // namespace zephyr::threading

#endif // MT_PATH_TRACER