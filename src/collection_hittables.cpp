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
    HitRecord hitrec({0, 0, 0}, -1, Vec3d(0, 0, 0), material, ray.dir);

    Vec3d c_o = ray.orig - center;
    double disc = radius * radius - (c_o.dot(c_o) - ray.dir.dot(c_o) * ray.dir.dot(c_o));
    if (disc < 0) {
        return hitrec;
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
        return hitrec;
    }

    Vec3d point = ray.cast(d);
    hitrec = HitRecord(point, d, this->normal(point), material, ray.dir);
    get_surface_coords(point, hitrec.surf_x, hitrec.surf_y);
    return hitrec;
}

bool h_Sphere::bounding_box(AABB &box) const {
    Vec3d radvec = Vec3d(radius, radius, radius);
    box.mn = center - radvec;
    box.mx = center + radvec;
    return true;
}

bool h_Sphere::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    Vec3d norm = normal(point);
    double theta = acos(-norm.y);
    double phi = atan2(-norm.z, norm.x) + VEC3D_PI;

    sx = phi / (2 * VEC3D_PI);
    sy = theta / VEC3D_PI;
    return true;
}

//=============================================================================

h_RectXY::h_RectXY():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_RectXY::h_RectXY(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(std::min(p0_.x, p1_.x), std::min(p0_.y, p1_.y), p0_.z)),
p1(Vec3d(std::max(p0_.x, p1_.x), std::max(p0_.y, p1_.y), p1_.z)) // we can force p1.z = p0.z, but what for
{}

HitRecord h_RectXY::hit(Ray &ray) const {
    HitRecord hitrec({0, 0, 0}, -1, Vec3d(0, 0, 0), material, ray.dir);

    double t = (p0.z - ray.orig.z) / ray.dir.z;
    double x = ray.orig.x + t * ray.dir.x;
    double y = ray.orig.y + t * ray.dir.y;
    // fprintf(stderr, "t = %lg x = %lg y = %lg\n", t, x, y);
    if (t < 0 || x < p0.x || x > p1.x || y < p0.y || y > p1.y) {
        // fprintf(stderr, "no\n");
        return hitrec;
    }

    get_surface_coords({x, y, p0.z}, hitrec.surf_x, hitrec.surf_y);
    hitrec.dist = t;
    hitrec.p = ray.cast(hitrec.dist);
    hitrec.n = Vec3d(0, 0, 1);
    hitrec.set_normal_orientation(ray.dir);
    hitrec.mat = material;
    return hitrec;
}

bool h_RectXY::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(0, 0, VEC3_EPS), p1 + Vec3d(0, 0, VEC3_EPS));
    return true;
}

bool h_RectXY::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    sx = (point.x-p0.x) / (p1.x-p0.x);
    sy = (point.y-p0.y) / (p1.y-p1.y);
    return true;
}

//=============================================================================

h_RectXZ::h_RectXZ():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_RectXZ::h_RectXZ(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(std::min(p0_.x, p1_.x), p0_.y, std::min(p0_.z, p1_.z))),
p1(Vec3d(std::max(p0_.x, p1_.x), p1_.y, std::max(p0_.z, p1_.z))) // we can force p1.z = p0.z, but what for
{}

HitRecord h_RectXZ::hit(Ray &ray) const {
    HitRecord hitrec({0, 0, 0}, -1, Vec3d(0, 0, 0), material, ray.dir);

    double t = (p0.y - ray.orig.y) / ray.dir.y;
    double x = ray.orig.x + t * ray.dir.x;
    double z = ray.orig.z + t * ray.dir.z;
    if (t < 0 || x < p0.x || x > p1.x || z < p0.z || z > p1.z) {
        return hitrec;
    }

    get_surface_coords({x, p0.y, z}, hitrec.surf_x, hitrec.surf_y);
    hitrec.dist = t;
    hitrec.p = {x, p0.y, z};
    hitrec.n = Vec3d(0, 1, 0);
    hitrec.set_normal_orientation(ray.dir);
    hitrec.mat = material;
    return hitrec;
}

bool h_RectXZ::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(0, VEC3_EPS, 0), p1 + Vec3d(0, VEC3_EPS, 0));
    return true;
}

bool h_RectXZ::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    sx = (point.x-p0.x) / (p1.x-p0.x);
    sy = (point.z-p0.z) / (p1.z-p1.z);
    return true;
}

//=============================================================================

h_RectYZ::h_RectYZ():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_RectYZ::h_RectYZ(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(p0_.x, std::min(p0_.y, p1_.y), std::min(p0_.z, p1_.z))),
p1(Vec3d(p1_.x, std::max(p0_.y, p1_.y), std::max(p0_.z, p1_.z))) // we can force p1.z = p0.z, but what for
{}

HitRecord h_RectYZ::hit(Ray &ray) const {
    HitRecord hitrec({0, 0, 0}, -1, Vec3d(0, 0, 0), material, ray.dir);

    double t = (p0.x - ray.orig.x) / ray.dir.x;
    double y = ray.orig.y + t * ray.dir.y;
    double z = ray.orig.z + t * ray.dir.z;
    if (t < 0 || y < p0.y || y > p1.y || z < p0.z || z > p1.z) {
        return hitrec;
    }

    get_surface_coords({p0.x, y, z}, hitrec.surf_x, hitrec.surf_y);
    hitrec.dist = t;
    hitrec.n = Vec3d(1, 0, 0);
    hitrec.set_normal_orientation(ray.dir);
    hitrec.mat = material;
    hitrec.p = ray.cast(hitrec.dist);
    return hitrec;
}

bool h_RectYZ::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(VEC3_EPS, 0, 0), p1 + Vec3d(VEC3_EPS, 0, 0));
    return true;
}

bool h_RectYZ::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    sx = (point.y-p0.y) / (p1.y-p0.y);
    sy = (point.z-p0.z) / (p1.z-p1.z);
    return true;
}

//=============================================================================

h_Box::h_Box():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_Box::h_Box(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(std::min(p0_.x, p1_.x), std::min(p0_.y, p1_.y), std::min(p0_.z, p1_.z))),
p1(Vec3d(std::max(p0_.x, p1_.x), std::max(p0_.y, p1_.y), std::max(p0_.z, p1_.z)))
{
    sides.insert(new h_RectYZ({p0.x, p0.y, p0.z}, {p1.x, p1.y, p1.z}, material_));
    sides.insert(new h_RectYZ({p1.x, p0.y, p0.z}, {p1.x, p1.y, p1.z}, material_));

    sides.insert(new h_RectXZ({p0.x, p0.y, p0.z}, {p1.x, p1.y, p1.z}, material_));
    sides.insert(new h_RectXZ({p0.x, p1.y, p0.z}, {p1.x, p1.y, p1.z}, material_));

    sides.insert(new h_RectXY({p0.x, p0.y, p0.z}, {p1.x, p1.y, p1.z}, material_));
    sides.insert(new h_RectXY({p0.x, p1.y, p1.z}, {p1.x, p1.y, p1.z}, material_));
}

HitRecord h_Box::hit(Ray &ray) const {
    return sides.hit(ray);
}

bool h_Box::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(VEC3_EPS, VEC3_EPS, VEC3_EPS), p1 + Vec3d(VEC3_EPS, VEC3_EPS, VEC3_EPS));
    return true;
}

bool h_Box::get_surface_coords(const Vec3d &, double &, double &) const {
    return false;
}