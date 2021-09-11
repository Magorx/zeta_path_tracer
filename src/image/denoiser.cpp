#include "denoiser.h"

Color get_denoised_color(const Image<Color> &image, const int x_center, const int y_center, const int sample_rad) {
    Color sum_color(0, 0, 0);
    double sum_weight = 0;

    for (int x = x_center - sample_rad; x < x_center + sample_rad; ++x) {
        for (int y = y_center - sample_rad; y < y_center + sample_rad; ++y) {
            sum_weight += 1;
            sum_color += image[x][y];
        }
    }

    return sum_color / sum_weight;
}

void denoise(const Image<Color> &input, Image<Color> &output, const int sample_rad) {
    if (&input == &output) {
        printf("[ERR] denoiser got input == output image, can't handle that way, sorry\n");
        return;
    }

    if (!input.same_size(output)) {
        printf("[ERR] denoiser got input.size != output.size\n");
        return;
    }

    for (int x = sample_rad; x < input.x_size - sample_rad; ++x) {
        for (int y = sample_rad; y < input.y_size - sample_rad; ++y) {
            output[x][y] = get_denoised_color(input, x, y, sample_rad);
        }
    }
}