#include "color.h"

const int 	 i_MAXRGB = 255;
const double d_MAXRGB = 255.0;
const int 	 i_MINRGB = 0;
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
	double rgb[3] = {color.x, color.y, color.z};
	for (int i = 0; i < 3; ++i) {
		if (rgb[i] < 0) {
			rgb[i] = 0;
		} else if (rgb[i] > 255) {
			rgb[i] = 255;
		}
	}

	return {rgb[0], rgb[1], rgb[2]};
}

Color shift_color_randomly(Color &color, const size_t module) {
	color = rand_shifted_color(color, module);
	return color;
}

bool is_valid_color(Color color) {
	return (color.x >= 0 && color.x <= 255 && color.y >= 0 && color.y <= 255 && color.z >= 0 && color.z <= 255);
}

Color clamped_rgb(const Color color) {
	return {std::min(std::max(color.x, 0.0), 255.0),
			std::min(std::max(color.y, 0.0), 255.0),
			std::min(std::max(color.z, 0.0), 255.0)
		   };
}

void print_rgb(Color color, const double gamma_correction, FILE *file) {
	color = clamped_rgb(color);
	color *= 1 / d_MAXRGB;
	color = pow(color, gamma_correction);
	color *= d_MAXRGB;
	fprintf(file, "%d %d %d\n", (int) color.x, (int) color.y, (int) color.z);
}

void save_rgb_to_ppm_image(FILE *fout, const Color *image, const size_t width, const size_t height, const double gamma_correction) {
	if (!fout) {
		fprintf(stderr, "[ERR]<save_rgb_to_ppm_image> fileptr[nullptr]\n");
		return;
	}
	if (!image) {
		fprintf(stderr, "[ERR]<save_rgb_to_ppm_image> image[nullptr]\n");
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
		fprintf(stderr, "[ERR]<save_rgb_to_ppm_image> filename[nullptr]\n");
		return;
	}

	FILE *fout = fopen(filename, "w");
	if (!fout) {
		fprintf(stderr, "[ERR]<save_rgb_to_ppm_image> can't open file[%s]\n", filename);
		return;
	}

	save_rgb_to_ppm_image(fout, image, width, height, gamma_correction);

	fclose(fout);
	return;
}