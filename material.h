#ifndef MATERIAL
#define MATERIAL

#include "ray.h"
#include "color.h"

struct HitRecord;

class Material {
public:
    virtual bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const = 0;
};

#endif // MATERIAL