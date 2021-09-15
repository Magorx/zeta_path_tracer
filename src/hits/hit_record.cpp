#include "hit_record.h"

const HitRecord HITREC_NONE({0, 0, 0}, -1, {0, 0, 0}, nullptr, {0, 0, 0});

HitRecord::HitRecord():
p(VEC3D_ZERO),
dist(0),
n(VEC3D_ZERO),
mat(nullptr),
surf_x(0),
surf_y(0)
{}

HitRecord::HitRecord(const Vec3d& point, double distance, const Vec3d& normal, Material* material, const Vec3d& ray_dir) :
p   (point),
dist(distance),
n   (normal),
mat (material),
surf_x(0),
surf_y(0)
{
    n.normalize();
	set_normal_orientation(ray_dir);
}

bool operator==(const HitRecord &first, const HitRecord &second) {
    return first.p == second.p && first.dist == second.dist && first.n == second.n;
}

bool operator<(const HitRecord &first, const HitRecord &second) {
    if (first.dist == -1) {
        return false;
    } else if (second.dist == -1) {
        return true;
    } else {
        return first.dist < second.dist;
    }
}
