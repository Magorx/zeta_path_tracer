#ifndef COLOR
#define COLOR

#include "vec3d.h"
#include <cstdio>

using Color = Vec3d;
using Point = Vec3d;

Color randcolor(const size_t module = 256);

Color rand_color_shift(const size_t module = 2);
Color rand_shifted_color(Color color, const size_t module = 2);
Color shift_color_randomly(Color &color, const size_t module = 2);

bool is_valid_color(Color color);

Color clamped_rgb(const Color color);

void print_rgb(Color color, FILE *file = stdout);

#endif // COLOR