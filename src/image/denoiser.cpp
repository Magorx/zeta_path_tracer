#include "denoiser.h"

Color get_denoised_color(const Frame<Color, Vec3d, double> &frame, const int x_center, const int y_center, const int sample_rad) {
    Color sum_color(0, 0, 0);
    double sum_weight = 0;

    for (int x = x_center - sample_rad; x < x_center + sample_rad; ++x) {
        for (int y = y_center - sample_rad; y < y_center + sample_rad; ++y) {
            sum_weight += 1;
            sum_color += frame.data_color[y * frame.size_x + x];
        }
    }

    return sum_color / sum_weight;
}

Color get_intelligence_denoised_color(const Frame<Color, Vec3d, double> &frame, const int x_center, const int y_center, const int sample_rad) {
    Color sum_color(0, 0, 0);
    double sum_weight = 0;

    int px = y_center * frame.size_x + x_center;

    Vec3d normal = frame.data_normal[px];
    double depth = frame.data_depth[px];

    for (int x = x_center - sample_rad; x < x_center + sample_rad; ++x) {
        for (int y = y_center - sample_rad; y < y_center + sample_rad; ++y) {
            if (x < 0 || x >= (int) frame.size_x || y < 0 || y >= (int) frame.size_y) {
                continue;
            }

            int cur_px = y * frame.size_x + x;

            Vec3d cur_normal = frame.data_normal[cur_px];
            double cur_depth = frame.data_depth[cur_px];

            double weight_depth  = 1 - std::abs(depth - cur_depth);
            double weight_normal = pow(std::max(0.0, normal.dot(cur_normal)), 3); // 128

            float w = weight_depth * weight_normal;
            
            sum_weight += w;
            sum_color += frame.data_color[y * frame.size_x + x] * w;
        }
    }

    return sum_color / sum_weight;
}

void denoise(Frame<Color, Vec3d, double> &frame, const int sample_rad) {
    for (size_t x = sample_rad; x < frame.size_x - sample_rad; ++x) {
        for (size_t y = sample_rad; y < frame.size_y - sample_rad; ++y) {
            size_t px_shift = y * frame.size_x + x;
            frame.final_image[px_shift] = get_denoised_color(frame, x, y, sample_rad);
        }
    }
}

void intelligence_denoise(Frame<Color, Vec3d, double> &frame, const int sample_rad) {
    for (size_t x = 0; x < frame.size_x; ++x) {
        for (size_t y = 0; y < frame.size_y; ++y) {
            size_t px_shift = y * frame.size_x + x;
            frame.final_image[px_shift] = get_intelligence_denoised_color(frame, x, y, sample_rad);
        }
    }
}
