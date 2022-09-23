#pragma once

#include "extern/argparse.h"

#include "utils/vec3d.h"

namespace zephyr::tracer::config {

struct RenderT {
    struct ScreenT {
        int width;
        int height;
    } screen;

    int max_trace_depth;
    int pixel_sampling;
    double gamma_correction;
    Vec3d background_color;
    
    void argparse_args(argparse::ArgumentParser &program);
    void argparse_scan(argparse::ArgumentParser &program);
};

} // namespace zephyr::tracer::config
