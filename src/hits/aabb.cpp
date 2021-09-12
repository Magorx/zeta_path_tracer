#include "aabb.h"

AABB::AABB() :
        mn(VEC3D_ZERO),
        mx(VEC3D_ZERO) {}

AABB::AABB(const Vec3d minimum, const Vec3d maximum) :
        mn(minimum),
        mx(maximum) {}

bool AABB::hit(const Ray &ray, double t_min, double t_max) const {
    for(int dim_i = 0; dim_i < 3; dim_i++) {
        double invD = 1.0f / ray.dir[dim_i];
        double t0 = (mn[dim_i] - ray.orig[dim_i]) * invD;
        double t1 = (mx[dim_i] - ray.orig[dim_i]) * invD;
        if(invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if(t_max <= t_min)
            return false;
    }
    return true;
}

AABB surrounding_box(const AABB first, const AABB second) {
    Vec3d mn(fmin(first.mn[0], second.mn[0]),
             fmin(first.mn[1], second.mn[1]),
             fmin(first.mn[2], second.mn[2]));

    Vec3d mx(fmax(first.mx[0], second.mx[0]),
             fmax(first.mx[1], second.mx[1]),
             fmax(first.mx[2], second.mx[2]));

    return {mn, mx};
}

double AABB::effective_size() const {
    return fabs((mx[0] - mn[0])) +
           fabs((mx[1] - mn[1])) +
           fabs((mx[2] - mn[2]));
}