#include "render.h"

#include "settings.h"


namespace zephyr::tracer::config {

void RenderT::argparse_args(argparse::ArgumentParser &program) {
    program.add_optional_args_cut("\nConfig [render]:");

    program.add_argument("-W", "--screen_width")
        .help("Screen width in pixels")
        .default_value(PIXEL_SCREEN_WIDTH)
        .scan<'i', int>();
    
    program.add_argument("-H", "--screen_height")
        .help("Screen height in pixels")
        .default_value(PIXEL_SCREEN_HEIGHT)
        .scan<'i', int>();

    program.add_argument("-D", "--trace_depth")
        .help("Depth of trace recursion")
        .default_value(MAX_TRACE_DEPTH)
        .scan<'i', int>();

    program.add_argument("-s", "--sampling")
        .help("Number of rays per pixel")
        .default_value(PIXEL_SCREEN_WIDTH)
        .scan<'i', int>();

    program.add_argument("-g", "--gamma")
        .help("Value for gamma correction")
        .default_value(GAMMA_CORRECTION)
        .scan<'g', double>();
    
    program.add_argument("-R", "--background_r")
        .help("Red component of the background color,   [0, 255]")
        .default_value(0)
        .scan<'i', int>();
    
    program.add_argument("-G", "--background_g")
        .help("Green component of the background color, [0, 255]")
        .default_value(0)
        .scan<'i', int>();
    
    program.add_argument("-B", "--background_b")
        .help("Blue component of the background color,  [0, 255]")
        .default_value(0)
        .scan<'i', int>();
}

void RenderT::argparse_scan(argparse::ArgumentParser &program) {
    screen.width  = program.get<int>("--screen_width");
    screen.height = program.get<int>("--screen_height");

    max_trace_depth = program.get<int>("--trace_depth");
    pixel_sampling = program.get<int>("--sampling");
    gamma_correction = program.get<double>("--gamma");

    background_color._x(program.get<int>("--background_r") / 255.0);
    background_color._y(program.get<int>("--background_g") / 255.0);
    background_color._z(program.get<int>("--background_b") / 255.0);
}

} // namespace zephyr::tracer::config
