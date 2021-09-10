#ifndef SFML_INTERFACE
#define SFML_INTERFACE

#include <SFML/Graphics.hpp>

#include <cstring>

#include "utils/PathTracer.hpp"

class SFML_Interface {
    sf::RenderWindow  window;
    sf::Texture image_texture;
    sf::Sprite        image_sprite;

    Scene *scene;
    conf_PathTracer config;

    int scr_w;
    int scr_h;
    
    int pixel_cnt;
    int pixel_sampling_per_render;

    RGBA  *cur_image;
    RGBA  *new_image;

    Color *cur_image_colored;
    Color *new_image_colored;

    Vec3d *normal_map;

    int consecutive_frames_cnt;

    void render_frame_portion();
    void flush_to_texture();

    void handle_events();

public:
    SFML_Interface(const char *window_name, Scene *scene_, const conf_PathTracer config_, int scr_w_, int scr_h_, int pixel_sampling_per_render_=1);

    void flush_to_window();
    
    void tick();
    void run();
};

#endif // SFML_INTERFACE