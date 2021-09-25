#ifndef HITTABLE_PLANE_H
#define HITTABLE_PLANE_H

#include "utils_header.h"

struct Plane : Hittable {
    Vec3d p;
    Vec3d n;

    Plane();
    Plane(const Vec3d point, const Vec3d normal, Material *material_);

    bool hit(Ray &ray, HitRecord* hit_record) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

#endif // HITTABLE_PLANE_H
