#ifndef HITTABLE
#define HITTABLE

#include <cmath>
#include "material/material.h"
#include "utils/ray.h"
#include "hit_record.h"
#include "aabb.h"

struct Hittable {
	Material *material;

    Hittable();
    Hittable(Material *material_);

    virtual HitRecord hit(Ray &ray) const = 0;
    virtual bool bounding_box(AABB &box) const = 0;
    virtual bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const;
};

#endif // HITTABLE
