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
cur_image_colored(nullptr),

frame(),

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

    cur_image_colored = (Color*) calloc(pixel_cnt, sizeof(Color));
    if (!cur_image) {
        fprintf(stderr, "[ERR] Can't calloc buffer for new_image_colored [%dx%d]\n", img_size.x, img_size.y);
    }

    frame = Frame<Color, Vec3d, double>(img_size.x, img_size.y);

    image_sprite.setTexture(image_texture);
    image_sprite.setScale(scr_w / img_size.x, scr_h / img_size.y);
}

void SFML_Interface::render_frame_portion() {
    config.render.PIXEL_SAMPLING = 1;
    render_into_buffer(scene, config, frame.data_color, frame.data_normal, frame.data_depth);

    if (!consecutive_frames_cnt) {
        memcpy(cur_image_colored, frame.data_color, pixel_cnt * sizeof(Color));
    } else {
        double n = consecutive_frames_cnt;
        for (int i = 0; i < pixel_cnt; ++i) {
            cur_image_colored[i] = cur_image_colored[i] * ((n - 1.0) / n) + frame.data_color[i] /  n;
        }
    }

    frame.normalize_depth_map();
    sf::Vector2u img_size = image_texture.getSize();
    Image<Color> output(img_size.x, img_size.y);
    for (int x = 0; x < img_size.x; ++x) {
        for (int y = 0; y < img_size.y; ++y) {
            int px_shift = y * img_size.x + x;
            output[y][x] = Vec3d(255, 255, 255) * (1 - frame.data_depth[px_shift]);
        }
    }
    // denoise(Image<Color>{cur_image_colored, img_size.x, img_size.y}, output);

    color_to_rgb_buffer(output.data, cur_image, config.render.GAMMA_CORRECTION, pixel_cnt);

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
