#ifndef HITTABLE_SPHERE_H
#define HITTABLE_SPHERE_H

#include "utils_header.h"

struct h_Sphere : public Hittable {
    Vec3d center;
    double radius;

    h_Sphere();
    h_Sphere(const Vec3d center, double radius, Material *material_);

    Vec3d normal(const Vec3d &point) const;
    bool hit(Ray &ray, HitRecord* hit_record) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

#endif // HITTABLE_SPHERE_H
