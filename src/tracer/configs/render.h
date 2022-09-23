#pragma once


#include "utils/vec3d.h"


namespace zephyr::tracer::config {

struct RenderT {
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int MAX_TRACE_DEPTH;
    int PIXEL_SAMPLING;
    double GAMMA_CORRECTION;
    Vec3d BACKGROUND_COLOR;

    RenderT(const int screen_width, const int screen_height,
            const int max_tracing_depth,
            const int pixel_sampling,
            const double gamma_correction,
            const Vec3d background_color);
};

} // namespace zephyr::tracer::config
