#include "collection_lights.h"

l_Diffuse::l_Diffuse(const Color color_):
color(color_)
{}

Vec3d l_Diffuse::emit(double surf_x, double surf_y, const Vec3d point) const {
	return color;
}