#include "mt_path_tracer.h"

void render_threaded(ThreadRenderTask &task) {
    for (int x = task.rtask.min_x; x < task.rtask.max_x; ++x) {
        for (int y = task.rtask.min_y; y < task.rtask.max_y; ++y) {
            int px_shift = y * task.frame.size_x + x;
            task.frame.data_color[px_shift] = accumulate_pixel_color(task.scene.camera, x, y, task.scene.objects, task.config, task.frame.data_normal + px_shift, task.frame.data_depth + px_shift);
        }
    }
}
