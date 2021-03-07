#ifndef PATH_TRACER
#define PATH_TRACER

#include <cstdio>
#include "vec3d.h"
#include "color.h"
#include "progress_bar.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "hit_record.h"
// #include "model.h"
// #include "light.h"
#include "camera.h"
// #include "scene.h"

// extern const double AMBIENT;
// extern const Vec3d BACKGROUND;

struct conf_Render {
	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
	const int MAX_TRACE_DEPTH;
	const int PIXEL_SAMPLING;
	const double GAMMA_CORRECTION;
	const Vec3d BACKGROUND_COLOR;

	conf_Render(const int screen_width, const int screen_height,
				const int max_tracing_depth,
				const int pixel_sampling,
				const double gamma_correction,
				const Vec3d background_color);
};

struct conf_Verbosity {
	const int PERCENT_STEP;
	const double PROGRESS_BAR_SCALE;

	conf_Verbosity(const int percent_step);
};

struct conf_PathTracer {
	const conf_Render 	 render;
	const conf_Verbosity verbos;

	conf_PathTracer(const conf_Render render_config, const conf_Verbosity verbos_config);
};

// Intersection test_ray(Ray &ray, const vector<Hittable*> &objects, const Hittable *to_ignore);
Vec3d trace_ray(Ray &ray, const Hittable *hittable, const conf_PathTracer &config, const int cur_trace_depth = 1);

Vec3d accumulate_pixel_color(const Camera *camera, const int px_x, const int px_y, 
							 const Hittable *hittable, const conf_PathTracer &config);

void render_image(Camera *camera, const Hittable *hittable, const conf_PathTracer &config);
// void save_image(vector<vector<Vec3d>> &image, string filename);

#endif // PATH_TRACER
