#include "rect_xy.h"

h_RectXY::h_RectXY():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_RectXY::h_RectXY(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(std::min(p0_[0], p1_[0]), std::min(p0_[1], p1_[1]), p0_[2])),
p1(Vec3d(std::max(p0_[0], p1_[0]), std::max(p0_[1], p1_[1]), p1_[2])) // we can force p1[2] = p0[2], but what for
{}

bool h_RectXY::hit(Ray &ray, HitRecord* hit_record) const {

    double t = (p0[2] - ray.orig[2]) / ray.dir[2];
    if(t > hit_record->dist) return false;

    double x = ray.orig[0] + t * ray.dir[0];
    double y = ray.orig[1] + t * ray.dir[1];
    if (t < 0 || x < p0[0] || x > p1[0] || y < p0[1] || y > p1[1] || t > hit_record->dist) {
        return false;
    }

    get_surface_coords({x, y, p0[2]}, hit_record->surf_x, hit_record->surf_y);
    hit_record->dist = t;
    hit_record->point = ray.cast(hit_record->dist);
    hit_record->normal = Vec3d(0, 0, 1);
    hit_record->set_normal_orientation(ray.dir);
    hit_record->mat = material;

    return true;
}

bool h_RectXY::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(0, 0, VEC3_EPS), p1 + Vec3d(0, 0, VEC3_EPS));
    return true;
}

bool h_RectXY::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    sx = (point[0]-p0[0]) / (p1[0]-p0[0]);
    sy = (point[1]-p0[1]) / (p1[1]-p1[1]);
    return true;
}
