#ifndef PATH_TRACER
#define PATH_TRACER

#include <cstdio>
#include <vector>

#include "vec3d.h"
#include "ray.h"
#include "color.h"

#include "progress_bar.h"

#include "hittable.h"
#include "hittable_list.h"
#include "hit_record.h"

#include "scene.h"

#include "bvh.h"

#include "render_task.h"
#include <thread>

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
	int VERBOSITY;

	conf_Verbosity(const int verbosity);
};

struct conf_SystemInfo {
	int  timestamp;
	int  kernel_cnt;
	const char *rtask_filename;

	conf_SystemInfo(const int timestamp_, const int kernel_cnt_, const char *rtask_filename_);
};

struct conf_PathTracer {
	const conf_Render 	  render;
	const conf_Verbosity  verbos;
	conf_SystemInfo sysinf;

	conf_PathTracer(const conf_Render 	  &render_config,
					const conf_Verbosity  &verbos_config,
					const conf_SystemInfo &sysinf_config);
};

// Intersection test_ray(Ray &ray, const vector<Hittable*> &objects, const Hittable *to_ignore);
Vec3d trace_ray(Ray &ray, const Hittable *hittable, const conf_PathTracer &config, const int cur_trace_depth = 1);

Vec3d accumulate_pixel_color(const Camera *camera, const int px_x, const int px_y, 
							 const Hittable *hittable, const conf_PathTracer &config);

void render_image       (Scene *scene, const conf_PathTracer &config);
void render_into_buffer (Scene *scene, const conf_PathTracer &config, Color *buffer);
void render_rtask       (Scene *scene, const conf_PathTracer &config, const RenderTask rtask, Color *buffer, const int verbouse = 1);

void render_from_rtask_file(Scene *scene, const conf_PathTracer &config);

#endif // PATH_TRACER
