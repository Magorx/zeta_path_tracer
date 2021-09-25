#include "diffuse.h"

l_Diffuse::l_Diffuse(const Color color_):
color(color_)
{}

Vec3d l_Diffuse::emit(double, double, const Vec3d&) const {
	return color;
}
