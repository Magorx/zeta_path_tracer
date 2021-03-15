#include "path_tracer.h"

Vec3d trace_ray(Ray &ray, const Hittable *hittable, const conf_PathTracer &config, const int cur_trace_depth) {
	ray.orig += ray.dir * VEC3_EPS;
	HitRecord hitrec = hittable->hit(ray);

	if (hitrec.dist < 0) {
		return config.render.BACKGROUND_COLOR;
	} else {
		if (cur_trace_depth == config.render.MAX_TRACE_DEPTH) {
			return {0, 0, 0};
		}

		Ray scattered_ray;
		Color attenuation;
		Color emmited = hitrec.mat->emit(hitrec.surf_x, hitrec.surf_y, hitrec.p);
		if (hitrec.mat->scatter(ray, hitrec, attenuation, scattered_ray)) {
			return emmited + attenuation * trace_ray(scattered_ray, hittable, config, cur_trace_depth + 1) / d_MAXRGB;
		} else {
			return emmited;
		}
	}
}

Vec3d accumulate_pixel_color(const Camera *camera, const int px_x, const int px_y, 
							 const Hittable *hittable, const conf_PathTracer &config) {
	Vec3d accumulator(0, 0, 0);
	for (int sample_i = 0; sample_i < config.render.PIXEL_SAMPLING; ++sample_i) {
		Ray sample_ray = camera->get_sample_ray((double) px_x, (double) px_y);
		accumulator += trace_ray(sample_ray, hittable, config);
	}
	return accumulator / config.render.PIXEL_SAMPLING;
}

void render_image(Camera *camera, const Hittable *hittable, const conf_PathTracer &config) {
	ProgressBar prog_bar(stderr, camera->res_h * camera->res_w, config.verbos.PROGRESS_BAR_SCALE);

    printf("P3 %d %d\n%d\n", config.render.SCREEN_WIDTH, config.render.SCREEN_HEIGHT, i_MAXRGB);

    prog_bar.start();
    for (int y = 0; y < camera->res_h; ++y) {
        for (int x = 0; x < camera->res_w; ++x) {
            prog_bar.tick();
            print_rgb(accumulate_pixel_color(camera, x, y, hittable, config), config.render.GAMMA_CORRECTION);
        }
    }

    fprintf(stderr, "doned.\n");
}

conf_Render::conf_Render(const int screen_width, const int screen_height,
						 const int max_tracing_depth,
						 const int pixel_sampling,
						 const double gamma_correction,
						 const Vec3d background_color):
SCREEN_WIDTH(screen_width),
SCREEN_HEIGHT(screen_height),
MAX_TRACE_DEPTH(max_tracing_depth),
PIXEL_SAMPLING(pixel_sampling),
GAMMA_CORRECTION(gamma_correction),
BACKGROUND_COLOR(background_color)
{}

conf_Verbosity::conf_Verbosity(const int percent_step):
PERCENT_STEP(percent_step),
PROGRESS_BAR_SCALE((1.0 * percent_step) / 100.0)
{}

conf_PathTracer::conf_PathTracer(const conf_Render render_config, const conf_Verbosity verbos_config):
render(render_config),
verbos(verbos_config)
{}