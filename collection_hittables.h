#ifndef COLLECTION_HITTABLES
#define COLLECTION_HITTABLES

#include "hittable.h"

struct h_Sphere : public Hittable {
    Vec3d center;
    double radius;

    h_Sphere();
    h_Sphere(const Vec3d center, double radius, Material *material_);

    Vec3d normal(const Vec3d &point) const;
    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
};

struct Plane : Hittable {
    Vec3d p;
    Vec3d n;

    Plane();
    Plane(const Vec3d point, const Vec3d normal, Material *material_);

    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
};

struct Triangle : Hittable {
    Vec3d p1;
    Vec3d p2;
    Vec3d p3;
    Vec3d n;
    Plane pl;
    double sq;

    Triangle();
    Triangle(const Vec3d point_1, const Vec3d point_2, const Vec3d point_3, Material *material_);

    bool is_point_inside(const Vec3d &p);
    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
};

#endif // COLLECTION_HITTABLES