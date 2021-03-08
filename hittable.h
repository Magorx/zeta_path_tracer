#ifndef HITTABLE
#define HITTABLE

#include <cmath>
#include "material.h"
#include "ray.h"
#include "hit_record.h"
#include "aabb.h"

struct Hittable {
    Hittable();
    Hittable(Material *material_);

    Material *material;
    virtual HitRecord hit(Ray &ray) const = 0;
    virtual bool bounding_box(AABB &box) const = 0;
};

#endif // HITTABLE
