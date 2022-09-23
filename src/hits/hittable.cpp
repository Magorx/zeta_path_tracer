#include "hittable.h"

Hittable::Hittable():
material(nullptr)
{}

Hittable::Hittable(Material *material_):
material(material_)
{}

bool Hittable::get_surface_coords(const Vec3d&, double &sx, double &sy) const {
    sx = 0;
    sy = 0;
    return false;
}

Hittable* Hittable::get_bvh_tree() {
    return this;
}
