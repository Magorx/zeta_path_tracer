#pragma once


#include "utils/progress_bar.h"

#include "hits/hittable.h"
#include "hits/hittable_list.h"
#include "hits/hit_record.h"

#include "camera_light_scene/scene.h"

#include "acceleration_structures/bvh.h"

#include "render_task.h"

#include "configs/full.h"

#include <cstdio>
#include <cstring>
#include <thread>
#include <vector>
#include <unistd.h>

namespace zephyr::tracer {

// Intersection test_ray(Ray &ray, const vector<Hittable*> &objects, const Hittable *to_ignore);
Vec3d trace_ray(Ray &ray, const Hittable *hittable, const config::FullT &config, const int cur_trace_depth = 1);

Vec3d trace_ray(Ray &ray, const Hittable *hittable, const config::FullT &config, const int cur_trace_depth,
                Vec3d *normal_vec, double *depth);

Vec3d accumulate_pixel_color(const Camera *camera, const int px_x, const int px_y, 
                             const Hittable *hittable, const config::FullT &config, 
                             Vec3d *normal_vec = nullptr, double *depth = nullptr);

void render_image       (Scene *scene, const config::FullT &config);

void render_into_buffer (Scene *scene, const config::FullT &config, Color *buffer);
void render_into_buffer (Scene *scene, const config::FullT &config, Color *buffer, Vec3d *normal_map, double *depth_map);

void render_rtask          (Scene *scene, const config::FullT &config, const RenderTask rtask, Color *buffer, const int verbouse = 1);
void render_from_rtask_file(Scene *scene, const config::FullT &config);

} // namespace zephyr::tracer
