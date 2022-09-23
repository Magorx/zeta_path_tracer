#include "path_tracer.h"

#include <utils/logger.h>


namespace zephyr::tracer {

Vec3d trace_ray(Ray &ray, const Hittable *hittable, const config::FullT &config, const int cur_trace_depth) {
    ray.orig += ray.dir * VEC3_EPS;
    HitRecord hitrec;
    hitrec.dist = (double) INT64_MAX;
    hittable->hit(ray, &hitrec);

    if (hitrec.dist == (double) INT64_MAX || hitrec.dist < 0) {
        return config.render.background_color;
    } else {
        if (cur_trace_depth == config.render.max_trace_depth) {
            return {0, 0, 0};
        }

        Ray scattered_ray;
        Color attenuation;
        Color emmited = hitrec.mat->emit(hitrec.surf_x, hitrec.surf_y, hitrec.point);
        if (hitrec.mat->scatter(ray, hitrec, attenuation, scattered_ray)) {
            attenuation *= trace_ray(scattered_ray, hittable, config, cur_trace_depth + 1);
            attenuation /= d_MAXRGB;
            emmited += attenuation;
        }

        return emmited;
    }
}

Vec3d trace_ray(Ray &ray, const Hittable *hittable, const config::FullT &config, const int cur_trace_depth,
               Vec3d *normal_vec, double *depth) {
    ray.orig += ray.dir * VEC3_EPS;
    HitRecord hitrec;
    hitrec.dist = (double) INT64_MAX;
    hittable->hit(ray, &hitrec);
    hitrec.normal.normalize();

    if (hitrec.dist == (double) INT64_MAX || hitrec.dist < 0) {
        return config.render.background_color;
    } else {
        if (cur_trace_depth == config.render.max_trace_depth) {
            return {0, 0, 0};
        }

        if (normal_vec) {
            *normal_vec += hitrec.normal;
        }
        if (depth) {
            *depth += hitrec.dist;
        }

        Ray scattered_ray;
        Color attenuation;
        Color emmited = hitrec.mat->emit(hitrec.surf_x, hitrec.surf_y, hitrec.point);
        if (hitrec.mat->scatter(ray, hitrec, attenuation, scattered_ray)) {
            // return scattered_ray.dir * 255;
            attenuation *= trace_ray(scattered_ray, hittable, config, cur_trace_depth + 1);
            attenuation /= d_MAXRGB;
            emmited += attenuation;
        }
        return emmited;
    }
}

Vec3d accumulate_pixel_color(const Camera *camera, const int px_x, const int px_y, 
                             const Hittable *hittable, const config::FullT &config, 
                             Vec3d *normal, double *depth) {
    if (normal) {
        memset(normal, 0, sizeof(Vec3d));
    }
    if (depth) {
        memset(depth, 0, sizeof(Vec3d));
    }

    Vec3d accumulator(config.render.background_color);
    for (int sample_i = 0; sample_i < config.render.pixel_sampling - 1; ++sample_i) {
        Ray sample_ray = camera->get_sample_ray((double) px_x, (double) px_y);
        accumulator += trace_ray(sample_ray, hittable, config, 1);
    }

    Ray central_ray;
    if (config.render.pixel_sampling == 1) {
        central_ray = camera->get_sample_ray((double) px_x, (double) px_y);
    } else {
        central_ray = camera->get_ray((double) px_x, (double) px_y);
    }
    accumulator += trace_ray(central_ray, hittable, config, 1, normal, depth);

    if (normal) {
        *normal /= (double) config.render.pixel_sampling;
        normal->normalize();
    }
    if (depth) {
        *depth /= (double) config.render.pixel_sampling;
    }

    return accumulator / config.render.pixel_sampling;
}

void render_image(Scene *scene, const config::FullT &config) {
    kctf::ProgressBar prog_bar( 
        scene->camera->res_h * scene->camera->res_w,
        config.verbos.verbosity
    );

    int res_h = scene->camera->res_h;
    int res_w = scene->camera->res_w;

    prog_bar.start();
    for (int y = 0; y < res_h; ++y) {
        for (int x = 0; x < res_w; ++x) {
            prog_bar.tick();
            print_rgb(accumulate_pixel_color(scene->camera, x, y, scene->objects, config), config.render.gamma_correction);
        }
    }
}

void render_rtask(Scene *scene, const config::FullT &config, RenderTask rtask, Color *buffer, const int verbouse) {
    if (!rtask.is_valid()) {
        logger.error("Zephyr", "[ERR] RTask is broken\n");
        return;
    }

    int height = rtask.height();
    int width  = rtask.width();

    kctf::ProgressBar prog_bar(height * width, config.verbos.verbosity && verbouse);

    int min_x = std::max(rtask.min_x, 0);
    int min_y = std::max(rtask.min_y, 0);
    int max_y = std::min(rtask.max_y, scene->camera->res_h);
    int max_x = std::min(rtask.max_x, scene->camera->res_w);

    prog_bar.start();
    for (int y = min_y; y < max_y; ++y) {
        for (int x = min_x; x < max_x; ++x) {
            prog_bar.tick();
            Color px_color = accumulate_pixel_color(scene->camera, x, y, scene->objects, config);
            buffer[width * (y - min_y) + x] = px_color;
        }
    }
}

void render_into_buffer(Scene *scene, const config::FullT &config, Color *buffer) {
    kctf::ProgressBar prog_bar(
        scene->camera->res_h * scene->camera->res_w,
        config.verbos.verbosity
    );

    prog_bar.start();
    int res_h = scene->camera->res_h;
    int res_w = scene->camera->res_w;
    for (int y = 0; y < res_h; ++y) {
        for (int x = 0; x < res_w; ++x) {
            prog_bar.tick();
            Color px_color = accumulate_pixel_color(scene->camera, x, y, scene->objects, config);
            buffer[res_w * y + x] = px_color;
        }
    }
}

void render_into_buffer(Scene *scene, const config::FullT &config, Color *buffer, Vec3d *normal_map, double *depth_map) {
    if (!normal_map) {
        logger.error("Zephyr", "no normal_map provided to render_into_buffer, thow you passed something here\n");
    }

    kctf::ProgressBar prog_bar(
        scene->camera->res_h,
        config.verbos.verbosity
    );

    prog_bar.start();
    int res_h = scene->camera->res_h;
    int res_w = scene->camera->res_w;
    for (int y = 0; y < res_h; ++y) {
        prog_bar.tick();
        for (int x = 0; x < res_w; ++x) {

            int px_shift = res_w * y + x;

            Color px_color = accumulate_pixel_color(scene->camera, x, y, scene->objects, config, normal_map + px_shift, depth_map + px_shift);
            buffer[px_shift] = px_color;
        }
    }
}

void render_from_rtask_file(Scene *scene, const config::FullT &config) {
    RenderTask full_rtask(0, scene->camera->res_w, 0, scene->camera->res_h, 0);
    // if (config.sysinf.rtask_filename) {
    //     full_rtask.load(config.sysinf.rtask_filename);
    // }

    std::vector<Color> image_buffer(full_rtask.width() * full_rtask.height(), config.render.background_color);

    if (config.sysinf.kernel_cnt == 1) {
        render_rtask(scene, config, full_rtask, image_buffer.data());
        save_rgb_to_ppm_image("image.ppm", image_buffer.data(), full_rtask.width(), full_rtask.height(), config.render.gamma_correction);
    } else {
        int kernel_cnt = config.sysinf.kernel_cnt;
        auto rtasks = full_rtask.linear_split(kernel_cnt);

        int task_buffer_offset = rtasks[0].width() * rtasks[0].height();
        std::vector<std::thread> threads;
        for (int i = 0; i < config.sysinf.kernel_cnt; ++i) {
            threads.push_back(std::thread{render_rtask, scene, config, rtasks[i], image_buffer.data() + i * task_buffer_offset, !i});
        }
        for (int i = 0; i < kernel_cnt; ++i) {
            threads[i].join();
        }

        save_rgb_to_ppm_image("image.ppm", image_buffer.data(), full_rtask.width(), full_rtask.height(), config.render.gamma_correction);
    }

    // free(image_buffer);
}

} // namespace zephyr::tracer
