#ifndef THREAD_RENDER_TASK_H
#define THREAD_RENDER_TASK_H

#include "PathTracer.hpp"

struct ThreadRenderTask {
    Scene &scene;
    
    conf_PathTracer &config;
    RenderTask rtask;

    Frame<Color, Vec3d, double> &frame;

    inline ThreadRenderTask(Scene &scene, conf_PathTracer &config, RenderTask rtask, Frame<Color, Vec3d, double> &frame) :
    scene(scene),
    config(config),
    rtask(rtask),
    frame(frame)
    {}
};

#endif // THREAD_RENDER_TASK_H