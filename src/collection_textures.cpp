#include "collection_textures.h"

t_SolidColor::t_SolidColor():
color(VEC3D_ONE * d_MAXRGB)
{}

t_SolidColor::t_SolidColor(const Color &color_):
color(color_)
{}

Color t_SolidColor::value(double, double, const Vec3d&) const {
	return color;
}

// ============================================================================

t_Checkered::t_Checkered():
first (new t_SolidColor(VEC3D_ONE * d_MAXRGB)),
second(new t_SolidColor(VEC3D_ZERO)),
freq(VEC3D_ONE)
{}

t_Checkered::t_Checkered(const Color &first_, const Color &second_, const Vec3d &freq_):
first (new t_SolidColor(first_ )),
second(new t_SolidColor(second_)),
freq(freq_)
{}

t_Checkered::t_Checkered(Texture *first_, Texture *second_, const Vec3d &freq_):
first(first_),
second(second_),
freq(freq_)
{}

Color t_Checkered::value(double sx, double sy, const Vec3d &point) const {
	double sines = sin(point.x * freq.x)* sin(point.y * freq.y) * sin(point.z * freq.z);
	if (sines < 0) {
		return  first->value(sx, sy, point);
	} else {
		return second->value(sx, sy, point);
	}
}