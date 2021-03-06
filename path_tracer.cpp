#include "path_tracer.h"

Vec3d trace_ray(Ray &ray, const Hittable *hittable, const conf_PathTracer &config) {
	HitRecord hitrec = hittable->hit(ray);
	if (hitrec.dist < 0) {
		return {20, 20, 20};
	} else {
		return Vec3d(255, 255, 255) * -hitrec.n;
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
	ProgressBar prog_bar(stderr, config.render.SCREEN_HEIGHT * config.render.SCREEN_WIDTH, config.verbos.PROGRESS_BAR_SCALE);

    printf("P3 %d %d\n%d\n", config.render.SCREEN_WIDTH, config.render.SCREEN_HEIGHT, 255);

    prog_bar.start();
    for (int y = 0; y < config.render.SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < config.render.SCREEN_WIDTH; ++x) {
            prog_bar.tick();
            
           	Color color = accumulate_pixel_color(camera, x, y, hittable, config);

            print_rgb(color);
        }
    }
}

conf_Render::conf_Render(const int screen_width, const int screen_height, const int max_tracing_depth, const int pixel_sampling):
SCREEN_WIDTH(screen_width),
SCREEN_HEIGHT(screen_height),
MAX_TRACING_DEPTH(max_tracing_depth),
PIXEL_SAMPLING(pixel_sampling)
{}

conf_Verbosity::conf_Verbosity(const int percent_step):
PERCENT_STEP(percent_step),
PROGRESS_BAR_SCALE((1.0 * percent_step) / 100.0)
{}

conf_PathTracer::conf_PathTracer(const conf_Render render_config, const conf_Verbosity verbos_config):
render(render_config),
verbos(verbos_config)
{}