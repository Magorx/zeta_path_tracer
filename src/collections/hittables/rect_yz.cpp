#include "rect_yz.h"

h_RectYZ::h_RectYZ():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_RectYZ::h_RectYZ(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(p0_[0], std::min(p0_[1], p1_[1]), std::min(p0_[2], p1_[2]))),
p1(Vec3d(p1_[0], std::max(p0_[1], p1_[1]), std::max(p0_[2], p1_[2]))) // we can force p1[2] = p0[2], but what for
{}

bool h_RectYZ::hit(Ray &ray, HitRecord* hit_record) const {

    double t = (p0[0] - ray.orig[0]) / ray.dir[0];
    if(t > hit_record->dist) return false;

    double y = ray.orig[1] + t * ray.dir[1];
    double z = ray.orig[2] + t * ray.dir[2];
    if (t < 0 || y < p0[1] || y > p1[1] || z < p0[2] || z > p1[2] || t > hit_record->dist) {
        return false;
    }

    get_surface_coords({p0[0], y, z}, hit_record->surf_x, hit_record->surf_y);
    hit_record->dist = t;
    hit_record->normal = Vec3d(1, 0, 0);
    hit_record->set_normal_orientation(ray.dir);
    hit_record->mat = material;
    hit_record->point = ray.cast(hit_record->dist);
    return true;
}

bool h_RectYZ::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(VEC3_EPS, 0, 0), p1 + Vec3d(VEC3_EPS, 0, 0));
    return true;
}

bool h_RectYZ::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    sx = (point[1]-p0[1]) / (p1[1]-p0[1]);
    sy = (point[2]-p0[2]) / (p1[2]-p0[2]);
    return true;
}
