#pragma once

#include "tracer/path_tracer.h"
#include "image/frame.h"

namespace zephyr::threading {

struct ThreadRenderTaskT {
    Scene &scene;
    
    zephyr::tracer::config::FullT &config;
    RenderTask rtask;

    Frame<Color, Vec3d, double> &frame;

    inline ThreadRenderTaskT(Scene &scene, zephyr::tracer::config::FullT &config, RenderTask rtask, Frame<Color, Vec3d, double> &frame) :
    scene(scene),
    config(config),
    rtask(rtask),
    frame(frame)
    {}
};

} // namespace zephyr::threading
