#ifndef COLLECTION_HITTABLES
#define COLLECTION_HITTABLES

#include "hittable.h"
#include "hittable_list.h"

struct h_Sphere : public Hittable {
    Vec3d center;
    double radius;

    h_Sphere();
    h_Sphere(const Vec3d center, double radius, Material *material_);

    Vec3d normal(const Vec3d &point) const;
    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

struct h_RectXY : public Hittable {
    Vec3d p0;
    Vec3d p1;

    h_RectXY();
    h_RectXY(const Vec3d &p0_, const Vec3d &p1_, Material *material_);

    Vec3d normal(const Vec3d &point) const;
    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

struct h_RectXZ : public Hittable {
    Vec3d p0;
    Vec3d p1;

    h_RectXZ();
    h_RectXZ(const Vec3d &p0_, const Vec3d &p1_, Material *material_);

    Vec3d normal(const Vec3d &point) const;
    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

struct h_RectYZ : public Hittable {
    Vec3d p0;
    Vec3d p1;

    h_RectYZ();
    h_RectYZ(const Vec3d &p0_, const Vec3d &p1_, Material *material_);

    Vec3d normal(const Vec3d &point) const;
    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

struct h_Box : public Hittable {
    Vec3d p0;
    Vec3d p1;
    HittableList sides;

    h_Box();
    h_Box(const Vec3d &p0_, const Vec3d &p1_, Material *material_);

    Vec3d normal(const Vec3d &point) const;
    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

struct Plane : Hittable {
    Vec3d p;
    Vec3d n;

    Plane();
    Plane(const Vec3d point, const Vec3d normal, Material *material_);

    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

struct Triangle : Hittable {
    Vec3d p0;
    Vec3d p1;
    Vec3d p2;

    Triangle();
    Triangle(const Vec3d &point_0, const Vec3d &point_1, const Vec3d &point_2, Material *material_);

    HitRecord hit(Ray &ray) const override;
    bool bounding_box(AABB &box) const override;
};

struct Model : public HittableList {
    Model(const char *filename, std::vector<Material*> matrs, const Vec3d &offset = {0, 0, 0}, const Vec3d &scale = {1, 1, 1});
    bool load(const char *filename, std::vector<Material*> matrs, const Vec3d &offset = {0, 0, 0}, const Vec3d &scale = {1, 1, 1});
};

#endif // COLLECTION_HITTABLES