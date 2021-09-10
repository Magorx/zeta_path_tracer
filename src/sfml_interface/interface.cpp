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
new_image(nullptr),

cur_image_colored(nullptr),
new_image_colored(nullptr),

normal_map(nullptr),

consecutive_frames_cnt(0)
{   
    if (!scene) {
        fprintf(stderr, "[ERR] scene is nullptr, aborting\n");
    }

    image_texture.create(scene->camera->res_w, scene->camera->res_h);
    sf::Vector2u img_size = image_texture.getSize();
    pixel_cnt = img_size.x * img_size.y;

    cur_image = (RGBA*) calloc(pixel_cnt, sizeof(RGBA));
    new_image = (RGBA*) calloc(pixel_cnt, sizeof(RGBA));
    if (!cur_image || !new_image) {
        fprintf(stderr, "[ERR] Can't calloc buffer for current/new image [%dx%d]\n", img_size.x, img_size.y);
    }

    cur_image_colored = (Color*) calloc(pixel_cnt, sizeof(Color));
    new_image_colored = (Color*) calloc(pixel_cnt, sizeof(Color));
    if (!cur_image || !new_image) {
        fprintf(stderr, "[ERR] Can't calloc buffer for new_image_colored [%dx%d]\n", img_size.x, img_size.y);
    }

    normal_map = (Vec3d*) calloc(pixel_cnt, sizeof(Vec3d));

    image_sprite.setTexture(image_texture);
    image_sprite.setScale(scr_w / img_size.x, scr_h / img_size.y);
}

void SFML_Interface::render_frame_portion() {
    config.render.PIXEL_SAMPLING = log(consecutive_frames_cnt + 1) + 1;
    render_into_buffer(scene, config, new_image_colored, normal_map);

    if (!consecutive_frames_cnt) {
        memcpy(cur_image, new_image, pixel_cnt * sizeof(RGBA));
    } else {
        double n = consecutive_frames_cnt;
        for (int i = 0; i < pixel_cnt; ++i) {
            cur_image_colored[i] = cur_image_colored[i] * ((n - 1.0) / n) + new_image_colored[i] /  n;
        }
    }

    color_to_rgb_buffer(cur_image_colored, cur_image, config.render.GAMMA_CORRECTION, pixel_cnt);

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