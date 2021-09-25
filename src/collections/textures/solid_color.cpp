#include "solid_color.h"

t_SolidColor::t_SolidColor():
color(VEC3D_ONE * d_MAXRGB)
{}

t_SolidColor::t_SolidColor(const Color &color_):
color(color_)
{}

Color t_SolidColor::value(double, double, const Vec3d&) const {
	return color;
}
