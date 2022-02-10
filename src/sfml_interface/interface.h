#ifndef SFML_INTERFACE
#define SFML_INTERFACE

#include <SFML/Graphics.hpp>

#include <cstring>
#include <string>

#include "PathTracer.hpp"
#include "image/frame.h"

#include "utils/timer.h"
#include "utils/logger.h"

class SFML_Interface { // a trashcan for everything, should be rewriten
    sf::RenderWindow window;
    sf::Texture      image_texture;
    sf::Sprite       image_sprite;

    Scene *scene;
    conf_PathTracer config;

    int scr_w;
    int scr_h;
    
    int pixel_cnt;
    int pixel_sampling_per_render;

    RGBA  *cur_image;
    Frame<Color, Vec3d, double> frame;
    Frame<Color, Vec3d, double> new_frame;

    int consecutive_frames_cnt;

    Threader<ThreadRenderTask> render_threader;

    long average_frame_ms;
    long average_frame_cnt;

    FramePostproc accumulator_frame_postproc;
    FramePostproc rendered_frame_postproc;

    double accumulator_frame_postproc_radius = 1;
    double rendered_frame_postproc_radius = 1;

    bool is_moving = false;
    bool render_done;

    void render_depth_buffer();
    void render_frame_portion();

    void accumulate_frame_portion();

    void render_frame_threaded();
    void flush_to_texture();

    void handle_events();
    void handle_movement();

public:
    bool is_run;

    SFML_Interface(const char *window_name, Scene *scene_, const conf_PathTracer config_, int scr_w_, int scr_h_, int pixel_sampling_per_render_=1);

    void flush_to_window();

    void screenshot_to_file(const char *filename = "scrsht.png");
    
    void tick();
    void run();

    void stop();

    inline void set_postprocs(FramePostproc accum_postproc    = FramePostproc::copy, 
                              FramePostproc rendered_postproc = FramePostproc::copy,
                              double accum_radius    = 1, 
                              double postproc_radius = 1) {
        accumulator_frame_postproc = accum_postproc;
        rendered_frame_postproc = rendered_postproc;
        accumulator_frame_postproc_radius = accum_radius;
        rendered_frame_postproc_radius = postproc_radius;
    }

    void interaction_loop();
};

#endif // SFML_INTERFACE