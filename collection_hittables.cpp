#include "collection_hittables.h"

h_Sphere::h_Sphere():
Hittable(),
center(0, 0, 0),
radius(0)
{}

h_Sphere::h_Sphere(const Vec3d center_, double radius_, Material *material_):
Hittable(material_),
center(center_),
radius(radius_)
{}

Vec3d h_Sphere::normal(const Vec3d &point) const {
	return ((point - center) * radius).normal();
}

HitRecord h_Sphere::hit(Ray &ray) const {
	Vec3d c_o = ray.orig - center;
    double disc = radius * radius - (c_o.dot(c_o) - ray.dir.dot(c_o) * ray.dir.dot(c_o));
    if (disc < 0) {
        return {{0, 0, 0}, -1, Vec3d(0, 0, 0), material, ray.dir};
    }

    double b = -ray.dir.dot(c_o);
    double d1 = b - sqrt(disc);
    double d2 = b + sqrt(disc);
    double d = -1;
    if (d1 > 0 && (d2 > d1 || d2 < 0)) {
        d = d1;
    } else if (d2 > 0 && (d1 > d2 || d1 < 0)) {
        d = d2;
    } else {
        return {{0, 0, 0}, -1, Vec3d(0, 0, 0), material, ray.dir};
    }

    Vec3d point = ray.cast(d);
    return {point, d, this->normal(point), material, ray.dir};
}
