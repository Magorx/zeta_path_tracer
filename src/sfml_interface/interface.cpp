#include "interface.h"

SFML_Interface::SFML_Interface(const char *window_name, Scene *scene_, const conf_PathTracer config_, int scr_w_, int scr_h_, int pixel_sampling_per_render_):
window(sf::VideoMode(scr_w_, scr_h_), window_name),
image_texture(),
image_sprite(),

scene(scene_),
config(config_),

scr_w(scr_w_),
scr_h(scr_h_),

pixel_cnt(0),
pixel_sampling_per_render(pixel_sampling_per_render_),

cur_image(nullptr),

frame(),
new_frame(),

consecutive_frames_cnt(0)
{   
    if (!scene) {
        fprintf(stderr, "[ERR] scene is nullptr, aborting\n");
    }

    image_texture.create(scene->camera->res_w, scene->camera->res_h);
    sf::Vector2u img_size = image_texture.getSize();
    pixel_cnt = img_size.x * img_size.y;

    cur_image = (RGBA*) calloc(pixel_cnt, sizeof(RGBA));
    if (!cur_image) {
        fprintf(stderr, "[ERR] Can't calloc buffer for current/new image [%dx%d]\n", img_size.x, img_size.y);
    }

    frame = Frame<Color, Vec3d, double>(img_size.x, img_size.y);
    new_frame = Frame<Color, Vec3d, double>(img_size.x, img_size.y);

    image_sprite.setTexture(image_texture);
    image_sprite.setScale((double) scr_w / img_size.x, (double) scr_h / img_size.y);
}

void SFML_Interface::render_frame_portion() {
    config.render.PIXEL_SAMPLING = 2;
    render_into_buffer(scene, config, new_frame.data_color, new_frame.data_normal, new_frame.data_depth);

    new_frame.normalize_depth_map();
    intelligence_denoise(new_frame, 1);
    // new_frame.colors_to_final_image();
    
    memcpy(frame.data_normal, new_frame.data_normal, frame.pixel_cnt * sizeof(Vec3d));
    memcpy(frame.data_depth, new_frame.data_depth, frame.pixel_cnt * sizeof(double));
    if (!consecutive_frames_cnt) {
        memcpy(frame.data_color, new_frame.data_color, pixel_cnt * sizeof(Color));
    } else {
        double n = consecutive_frames_cnt;
        for (int i = 0; i < pixel_cnt; ++i) {
            frame.data_color[i] = frame.data_color[i] * ((n - 1.0) / n) + new_frame.final_image[i] /  n;
        }
    }
    
    intelligence_denoise(frame, 2);
    // frame.colors_to_final_image();

    color_to_rgb_buffer(frame.final_image, cur_image, config.render.GAMMA_CORRECTION, pixel_cnt);

    ++consecutive_frames_cnt;
}

void SFML_Interface::flush_to_texture() {
    if (!scene || !scene->camera) {
        printf("[ERR] BAD scene or scene->camera in sfml_interface during flush_to_texture()");
    }

    int x_max = image_texture.getSize().x;
    int y_max = image_texture.getSize().y;

    image_texture.update((sf::Uint8*) cur_image);
}

void SFML_Interface::flush_to_window() {
    window.draw(image_sprite);
}

void SFML_Interface::handle_events() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void SFML_Interface::tick() {
    window.clear();

    render_frame_portion();

    flush_to_texture();

    flush_to_window();
}

void SFML_Interface::run() {
    while (window.isOpen())
    {
        handle_events();
        
        tick();

        window.display();
    }
}
