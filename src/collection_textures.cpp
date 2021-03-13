#include "collection_textures.h"

t_SolidColor::t_SolidColor():
color(VEC3D_ZERO)
{}

t_SolidColor::t_SolidColor(const Color &color_):
color(color_)
{}

Color t_SolidColor::value(double, double, const Vec3d&) const {
	return color;
}

// ============================================================================

t_Checkered::t_Checkered():
color(VEC3D_ZERO),
freq(VEC3D_ONE)
{}

t_Checkered::t_Checkered(const Color &color_, const Vec3d &freq_):
color(color_),
freq(freq_)
{}

Color t_Checkered::value(double, double, const Vec3d &point) const {
	return color * Vec3d::sign(sin(point.x * freq.x)) * Vec3d::sign(sin(point.y * freq.y)) * Vec3d::sign(sin(point.z * freq.z));
}