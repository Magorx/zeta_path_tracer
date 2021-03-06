#include "color.h"

Color randcolor(const size_t module) {
	return {(double) (rand() % module), (double) (rand() % module), (double) (rand() % module)};
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

void print_rgb(Color color, FILE *file) {
	color = clamped_rgb(color);
	fprintf(file, "%d %d %d\n", (int) color.x, (int) color.y, (int) color.z);
}
