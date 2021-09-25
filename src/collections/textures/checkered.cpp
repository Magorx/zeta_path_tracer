#include "checkered.h"
#include "solid_color.h"

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
	double sines = sin(point[0] * freq[0])* sin(point[1] * freq[1]) * sin(point[2] * freq[2]);
	if (sines < 0) {
		return  first->value(sx, sy, point);
	} else {
		return second->value(sx, sy, point);
	}
}
