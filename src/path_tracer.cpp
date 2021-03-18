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

void render_image(Scene *scene, const conf_PathTracer &config) {
	ProgressBar prog_bar(stderr, 
						 scene->camera->res_h * scene->camera->res_w,
						 config.verbos.VERBOSITY);

    printf("P3 %d %d\n%d\n", config.render.SCREEN_WIDTH, config.render.SCREEN_HEIGHT, i_MAXRGB);

    prog_bar.start();
    int res_h = scene->camera->res_h;
    int res_w = scene->camera->res_w;
    for (int y = 0; y < res_h; ++y) {
        for (int x = 0; x < res_w; ++x) {
            prog_bar.tick();
            print_rgb(accumulate_pixel_color(scene->camera, x, y, scene->objects, config), config.render.GAMMA_CORRECTION);
        }
    }
}

void render_rtask(Scene *scene, const conf_PathTracer &config, RenderTask rtask, Color *buffer, const int verbouse) {
	if (!rtask.is_valid()) {
		fprintf(stderr, "[ERR] RTask is broken\n");
		return;
	}

	int height = rtask.height();
    int width  = rtask.width();

	ProgressBar prog_bar(stderr, height * width, config.verbos.VERBOSITY && verbouse);

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

void render_into_buffer(Scene *scene, const conf_PathTracer &config, Color *buffer) {
	ProgressBar prog_bar(stderr, scene->camera->res_h * scene->camera->res_w,
						 config.verbos.VERBOSITY);

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

void render_from_rtask_file(Scene *scene, const conf_PathTracer &config) {
	RenderTask full_rtask(0, scene->camera->res_w, 0, scene->camera->res_h, 0);
	if (config.sysinf.rtask_filename) {
		full_rtask.load(config.sysinf.rtask_filename);
	}

	Color *image_buffer = (Color*) calloc(full_rtask.width() * full_rtask.height(), sizeof(Color));

	if (config.sysinf.kernel_cnt == 1) {
		render_rtask(scene, config, full_rtask, image_buffer);
		save_rgb_to_ppm_image("image.ppm", image_buffer, full_rtask.width(), full_rtask.height(), config.render.GAMMA_CORRECTION);
	} else {
		int kernel_cnt = config.sysinf.kernel_cnt;
		full_rtask.linear_split(kernel_cnt, config.sysinf.timestamp);

		if (config.verbos.VERBOSITY >= 2) printf("={ RenderTasks }=====\n");

		std::vector<RenderTask> rtasks;
		char rt_name[50];
		for (int i = 0; i < kernel_cnt; ++i) {
			sprintf(rt_name, "%d_%d.rt", i, config.sysinf.timestamp);
			
			RenderTask rtask;
			rtask.load(rt_name);
			if (config.verbos.VERBOSITY >= 2) {
				rtask.dump();
				if (i != kernel_cnt - 1) printf("-----\n");
			}
			rtasks.push_back(rtask);
		}

		if (config.verbos.VERBOSITY >= 2) printf("=====================\n");

		int task_buffer_offset = rtasks[0].width() * rtasks[0].height();
		std::thread **threads = (std::thread**) calloc(config.sysinf.kernel_cnt, sizeof(std::thread*));
		for (int i = 0; i < config.sysinf.kernel_cnt; ++i) {
			threads[i] = new std::thread(render_rtask, scene, config, rtasks[i], image_buffer + i * task_buffer_offset, !i);
		}

		for (int i = 0; i < kernel_cnt; ++i) {
			threads[i]->join();
			delete threads[i];
		}
		free(threads);

		save_rgb_to_ppm_image("image.ppm", image_buffer, full_rtask.width(), full_rtask.height(), config.render.GAMMA_CORRECTION);
	}
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

conf_Verbosity::conf_Verbosity(const int percent_step, const int verbosity):
PERCENT_STEP(percent_step),
PROGRESS_BAR_SCALE((1.0 * percent_step) / 100.0),
VERBOSITY(verbosity)
{}

conf_SystemInfo::conf_SystemInfo(const int timestamp_, const int kernel_cnt_, const char *rtask_filename_):
timestamp(timestamp_),
kernel_cnt(kernel_cnt_),
rtask_filename(rtask_filename_)
{}

conf_PathTracer::conf_PathTracer(const conf_Render     &render_config,
								 const conf_Verbosity  &verbos_config,
								 const conf_SystemInfo &sysinf_config):
render(render_config),
verbos(verbos_config),
sysinf(sysinf_config)
{}
