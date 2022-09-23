#include "hit_record.h"

const HitRecord HITREC_NONE({0, 0, 0}, -1, {0, 0, 0}, nullptr, {0, 0, 0});

HitRecord::HitRecord():
        point(VEC3D_ZERO),
        dist(0),
        normal(VEC3D_ZERO),
        mat(nullptr),
        surf_x(0),
        surf_y(0)
{}

HitRecord::HitRecord(const Vec3d& point, double distance, const Vec3d& normal, Material* material, const Vec3d& ray_dir) :
        point   (point),
        dist(distance),
        normal   (normal),
        mat (material),
        surf_x(0),
        surf_y(0)
{
    this->normal.normalize();
    set_normal_orientation(ray_dir);
}

bool operator==(const HitRecord &first, const HitRecord &second) {
    return first.point == second.point && first.dist == second.dist && first.normal == second.normal;
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
