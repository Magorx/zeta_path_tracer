#ifndef HITTABLE_TRIANGLE_H
#define HITTABLE_TRIANGLE_H

#include "utils_header.h"

struct Triangle : Hittable {
    Vec3d p0;

    Vec3d e1;
    Vec3d e2;

    Vec3d n1;
    Vec3d n2;
    Vec3d n3;
    bool to_phong_normals;

    inline Vec3d p1() const { return p0 + e1; }
    inline Vec3d p2() const { return p0 + e2; }
    inline void set_normals(const Vec3d &n1_, const Vec3d &n2_, const Vec3d &n3_, bool to_phong_normals_ = true) {
        n1 = n1_;
        n2 = n2_;
        n3 = n3_;
        to_phong_normals = to_phong_normals_;
    }
    
    Vec3d calc_normal(double u, double v) const;

    Triangle();
    Triangle(const Vec3d &point_0, const Vec3d &point_1, const Vec3d &point_2, Material *material_);

    bool hit(Ray &ray, HitRecord* hit_record) const override;
    bool bounding_box(AABB &box) const override;
};

#endif // HITTABLE_TRIANGLE_H
