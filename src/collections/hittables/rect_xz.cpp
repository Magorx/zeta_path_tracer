#include "rect_xz.h"

h_RectXZ::h_RectXZ():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_RectXZ::h_RectXZ(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(std::min(p0_[0], p1_[0]), p0_[1], std::min(p0_[2], p1_[2]))),
p1(Vec3d(std::max(p0_[0], p1_[0]), p1_[1], std::max(p0_[2], p1_[2]))) // we can force p1[2] = p0[2], but what for
{}

bool h_RectXZ::hit(Ray &ray, HitRecord* hit_record) const {

    double t = (p0[1] - ray.orig[1]) / ray.dir[1];
    if(t > hit_record->dist) return false;

    double x = ray.orig[0] + t * ray.dir[0];
    double z = ray.orig[2] + t * ray.dir[2];
    if (t < 0 || x < p0[0] || x > p1[0] || z < p0[2] || z > p1[2] || t > hit_record->dist) {
        return false;
    }

    get_surface_coords({x, p0[1], z}, hit_record->surf_x, hit_record->surf_y);
    hit_record->dist = t;
    hit_record->point = {x, p0[1], z};
    hit_record->normal = Vec3d(0, 1, 0);
    hit_record->set_normal_orientation(ray.dir);
    hit_record->mat = material;
    return true;
}

bool h_RectXZ::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(0, VEC3_EPS, 0), p1 + Vec3d(0, VEC3_EPS, 0));
    return true;
}

bool h_RectXZ::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    sx = (point[0]-p0[0]) / (p1[0]-p0[0]);
    sy = (point[2]-p0[2]) / (p1[2]-p1[2]);
    return true;
}
