#include "material.h"

Material::Material():
emitter(nullptr)
{}

void Material::set_emitter(Light *emitter_) {
	emitter = emitter_;
}

Vec3d Material::emit(double surf_x, double surf_y, const Vec3d point) const {
	return emitter ? emitter->emit(surf_x, surf_y, point) : VEC3D_ZERO;
}
