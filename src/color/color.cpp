#include "color.h"

#include <utils/logger.h>


const int      i_MAXRGB = 255;
const double d_MAXRGB = 255.0;
const int      i_MINRGB = 0;
const double d_MINRGB = 0.0;

Color randcolor(const int min, const int max) {
    return {(double) ((rand() % max) + min), (double) ((rand() % max) + min), (double) ((rand() % max) + min)};
}

Color rand_color_shift(const size_t module) {
    double rgb[3] = {};
    for (int i = 0; i < 3; ++i) {
        rgb[i] = (double) (rand() % module);
        rgb[i] *= rand() % 2 ? 1 : -1;
    }
    return {rgb[0], rgb[1], rgb[2]};
}

Color rand_shifted_color(Color color, const size_t module) {
    color += rand_color_shift(module);
    auto rgb = color;
    for (int i = 0; i < 3; ++i) {
        if (rgb[i] < 0) {
            rgb.set(i ,0);
        } else if (rgb[i] > 255) {
            rgb.set(i ,255);
        }
    }

    return {rgb[0], rgb[1], rgb[2]};
}

Color shift_color_randomly(Color &color, const size_t module) {
    color = rand_shifted_color(color, module);
    return color;
}

bool is_valid_color(Color color) {
    return (color.content[0] >= 0 && color.content[0] <= 255 && color.content[1] >= 0 && color.content[1] <= 255 && color.content[2] >= 0 && color.content[2] <= 255);
}

Color clamped_rgb(const Color color) {
    return {std::min(std::max(color.content[0], 0.0), 255.0),
            std::min(std::max(color.content[1], 0.0), 255.0),
            std::min(std::max(color.content[2], 0.0), 255.0)
           };
}

void print_rgb(Color color, const double gamma_correction, FILE *file) {
    RGBA rgba = color_to_final_rgba(color, gamma_correction);
    fprintf(file, "%d %d %d\n", (int) rgba.r, (int) rgba.g, (int) rgba.b);
}

void save_rgb_to_ppm_image(FILE *fout, const Color *image, const size_t width, const size_t height, const double gamma_correction) {
    if (!fout) {
        logger.error("color") << "save_rgb_to_ppm_image fileptr[nullptr]";
        return;
    }
    if (!image) {
        logger.error("color") << "save_rgb_to_ppm_image image[nullptr]";
        return;
    }

    fprintf(fout, "P3 %lu %lu\n%d\n", width, height, i_MAXRGB);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            print_rgb(image[width * y + x], gamma_correction, fout);
        }
    }
}

void save_rgb_to_ppm_image(const char *filename, const Color *image, const size_t width, const size_t height, const double gamma_correction) {
    if (!filename) {
        logger.error("color") << "save_rgb_to_ppm_image filename[nullptr]";
        return;
    }

    FILE *fout = fopen(filename, "w");
    if (!fout) {
        logger.error("color") << "save_rgb_to_ppm_image can't open file[" << filename << "]";
        return;
    }

    save_rgb_to_ppm_image(fout, image, width, height, gamma_correction);

    fclose(fout);
    return;
}

RGBA color_to_final_rgba(Color color, const double gamma_correction) {
    color = clamped_rgb(color);
    color *= 1 / d_MAXRGB;
    color = pow(color, gamma_correction);
    color *= d_MAXRGB;
    return {(unsigned char) color.content[0], (unsigned char) color.content[1], (unsigned char) color.content[2]};
}

void color_to_rgb_buffer(const Color *image, RGBA *buffer, const double gamma_correction, const int pixel_cnt) {
    for (int i = 0; i < pixel_cnt; ++i) {
        buffer[i] = color_to_final_rgba(image[i], gamma_correction);
    }
}

RGBA *mean_image(RGBA *first, const RGBA *second, const int pixel_cnt) {
    if (!first || !second) {
        return first;
    }

    for (int i = 0; i < pixel_cnt; ++i) {
        first[i] = (first[i] + second[i]) / 2;
    }

    return first;
}
