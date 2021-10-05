#ifndef HITTABLE_TRIANGLE_H
#define HITTABLE_TRIANGLE_H

#include "utils_header.h"

struct Triangle : Hittable {
    Vec3d p0;

    Vec3d e1;
    Vec3d e2;

    inline Vec3d p1() const { return p0 + e1; }
    inline Vec3d p2() const { return p0 + e2; }

    Triangle();
    Triangle(const Vec3d &point_0, const Vec3d &point_1, const Vec3d &point_2, Material *material_);

    bool hit(Ray &ray, HitRecord* hit_record) const override;
    bool bounding_box(AABB &box) const override;
};

#endif // HITTABLE_TRIANGLE_H
