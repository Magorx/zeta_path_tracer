#include "material.h"

Material::Material():
emitter(nullptr)
{}

void Material::set_emitter(Light *emitter_) {
    emitter = emitter_;
}

Vec3d Material::emit(const double surf_x, const double surf_y, const Vec3d &point) const {
    if (!emitter) {
        return VEC3D_ZERO;
    }

    Vec3d emitted = emitter->emit(surf_x, surf_y, point);
    affect_emitter(emitted, surf_x, surf_y, point);
    return emitted;
}

void Material::affect_emitter(Vec3d &,const double, const double, const Vec3d&) const {}
