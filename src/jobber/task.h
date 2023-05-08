#pragma once


#include <camera_light_scene/scene.h>
#include <color/color.h>
#include <image/image.h>
#include <utils/vec3d.h>

#include <tracer/configs/full.h>
#include <tracer/path_tracer.h>

#include <utils/logger.h>


namespace zephyr {

class TaskT {
public:
    struct ResultT {
        size_t x;
        size_t y;

        Image<Color> image;

        ResultT() 
        : x(0), y(0), image(0, 0)
        {}

        ResultT(size_t x_, size_t y_, size_t width, size_t height)
        : x(x_)
        , y(y_)
        , image(width, height)
        {}
    };

    struct ComputeContextT {
        Scene scene;
        zephyr::tracer::config::FullT config;
    };

    struct ComputeContextUpdateT {
        Scene scene;
        zephyr::tracer::config::FullT config;
    };

private:
    size_t x;
    size_t y;

    size_t width;
    size_t height;

public:
    TaskT(size_t x, size_t y, size_t width, size_t height)
        : x(x)
        , y(y)
        , width(width)
        , height(height)
    {}

public:
    ResultT run() {
        ResultT result(x, y, width, height);

        zephyr::tracer::render_into_buffer(
            &compute_context.scene,
            compute_context.config,
            &result.image[0],
            {
                (int) x,
                (int) (x + width),
                (int) y,
                (int) (y + height)
            },
            logger.none
        );

        return result;
    }

    size_t complexity() const {
        return width * height;
    }

    std::vector<TaskT> split(size_t n_tasks) const {
        std::vector<TaskT> tasks;

        if (n_tasks <= 1) {
            tasks.push_back(*this);
            return tasks;
        }

        if (n_tasks == 2) {
            if (width > height) {
                tasks.push_back(TaskT(x, y, width / 2, height));
                tasks.push_back(TaskT(x + width / 2, y, width - width / 2, height));
            } else {
                tasks.push_back(TaskT(x, y, width, height / 2));
                tasks.push_back(TaskT(x, y + height / 2, width, height - height / 2));
            }
            return tasks;
        }

        throw std::out_of_range("n_tasks must be 1 or 2");
    }

    static void update_compute_context(const ComputeContextUpdateT &update) {
        compute_context.config = update.config;
        compute_context.scene = update.scene;
    }

    static ResultT reduce(const std::vector<ResultT> &&results) {
        size_t min_x = results[0].x;
        size_t min_y = results[0].y;
        size_t max_x = results[0].x + results[0].image.width();
        size_t max_y = results[0].y + results[0].image.height();

        for (const auto &result : results) {
            min_x = std::min(min_x, result.x);
            min_y = std::min(min_y, result.y);
            max_x = std::max(max_x, result.x + result.image.width());
            max_y = std::max(max_y, result.y + result.image.height());
        }

        ResultT final_result(min_x, min_y, max_x - min_x, max_y - min_y);

        for (const auto &result : results) {
            for (size_t y = 0; y < result.image.height(); ++y) {
                for (size_t x = 0; x < result.image.width(); ++x) {
                    final_result.image(
                        result.x + x,
                        result.y + y
                    ) = result.image(x, y);
                }
            }
        }

        return final_result;
    }

    static TaskT empty() {
        return TaskT(0, 0, 0, 0);
    }

    bool is_empty() const {
        return width == 0 || height == 0;
    }

private:
    std::vector<int> data;

    static ComputeContextT compute_context;
};

// extern TaskT::ComputeContextT TaskT::compute_context;

} // namespace zephyr
