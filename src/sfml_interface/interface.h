#ifndef SFML_INTERFACE
#define SFML_INTERFACE

#include <SFML/Graphics.hpp>

#include <cstring>
#include <string>

#include "PathTracer.hpp"
#include "image/frame.h"

#include "utils/timer.h"
#include "utils/logger.h"

class SFML_Interface {
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

    void render_frame_portion();
    void render_frame_threaded();
    void flush_to_texture();

    void handle_events();

public:
    SFML_Interface(const char *window_name, Scene *scene_, const conf_PathTracer config_, int scr_w_, int scr_h_, int pixel_sampling_per_render_=1);

    void flush_to_window();

    void screenshot_to_file(const char *filename = "scrsht.png");
    
    void tick();
    void run();

    void stop();
};

#endif // SFML_INTERFACE