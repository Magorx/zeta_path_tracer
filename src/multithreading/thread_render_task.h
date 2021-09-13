#ifndef THREAD_RENDER_TASK_H
#define THREAD_RENDER_TASK_H

#include "PathTracer.hpp"

struct ThreadRenderTask {
    Scene &scene;
    
    const conf_PathTracer &config;
    RenderTask rtask;

    Frame<Color, Vec3d, double> &frame;
};

#endif // THREAD_RENDER_TASK_H