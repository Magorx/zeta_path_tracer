#include "sphere.h"

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
    Vec3d result = point;
    result -= center;
    result.normalize();
	return result;
}

bool h_Sphere::hit(Ray &ray, HitRecord* hit_record) const {

    Vec3d c_o = ray.orig - center;
    double disc = radius * radius - (c_o.dot(c_o) - ray.dir.dot(c_o) * ray.dir.dot(c_o));
    if (disc < 0) {
        return false;
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
        return false;
    }

    if(d > hit_record->dist) return false;

    Vec3d point = ray.cast(d);
    hit_record->mat = material;
    hit_record->set_normal_orientation(ray.dir);
    hit_record->point = point;
    hit_record->dist = d;
    hit_record->normal = this->normal(point);
    get_surface_coords(point, hit_record->surf_x, hit_record->surf_y);
    return true;
}

bool h_Sphere::bounding_box(AABB &box) const {
    Vec3d radvec = Vec3d(radius, radius, radius);
    box.mn = center - radvec;
    box.mx = center + radvec;
    return true;
}

bool h_Sphere::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    Vec3d norm = normal(point);
    double theta = acos(-norm[1]);
    double phi = atan2(-norm[2], norm[0]) + VEC3D_PI;

    sx = phi / (2 * VEC3D_PI);
    sy = theta / VEC3D_PI;
    return true;
}
