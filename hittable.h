#ifndef HITTABLE
#define HITTABLE

#include <cmath>
#include "material.h"
#include "ray.h"
#include "hit_record.h"

struct Hittable {
    Hittable();
    Hittable(Material *material_);

    Material *material;
    virtual HitRecord hit(Ray &ray) const = 0;
};

#endif // HITTABLE
