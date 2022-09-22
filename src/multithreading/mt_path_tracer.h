#ifndef MT_PATH_TRACER
#define MT_PATH_TRACER

#include "threader.h"
#include "thread_render_task.h"

void render_threaded(ThreadRenderTask &task);

#endif // MT_PATH_TRACER