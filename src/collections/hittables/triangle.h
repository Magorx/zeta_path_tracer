#ifndef HITTABLE_TRIANGLE_H
#define HITTABLE_TRIANGLE_H

#include "utils_header.h"

struct Triangle : Hittable {
    Vec3d p0;
    Vec3d p1;
    Vec3d p2;

    Triangle();
    Triangle(const Vec3d &point_0, const Vec3d &point_1, const Vec3d &point_2, Material *material_);

    bool hit(Ray &ray, HitRecord* hit_record) const override;
    bool bounding_box(AABB &box) const override;
};

#endif // HITTABLE_TRIANGLE_H
