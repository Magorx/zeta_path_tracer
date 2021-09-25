#include "box.h"

h_Box::h_Box():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_Box::h_Box(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(std::min(p0_[0], p1_[0]), std::min(p0_[1], p1_[1]), std::min(p0_[2], p1_[2]))),
p1(Vec3d(std::max(p0_[0], p1_[0]), std::max(p0_[1], p1_[1]), std::max(p0_[2], p1_[2])))
{
    sides.insert(new h_RectYZ({p0[0], p0[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));
    sides.insert(new h_RectYZ({p1[0], p0[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));

    sides.insert(new h_RectXZ({p0[0], p0[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));
    sides.insert(new h_RectXZ({p0[0], p1[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));

    sides.insert(new h_RectXY({p0[0], p0[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));
    sides.insert(new h_RectXY({p0[0], p0[1], p1[2]}, {p1[0], p1[1], p1[2]}, material_));
}

bool h_Box::hit(Ray &ray, HitRecord* hit_record) const {
    return sides.hit(ray, hit_record);
}

bool h_Box::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(VEC3_EPS, VEC3_EPS, VEC3_EPS), p1 + Vec3d(VEC3_EPS, VEC3_EPS, VEC3_EPS));
    return true;
}

bool h_Box::get_surface_coords(const Vec3d &, double &, double &) const {
    return false;
}
