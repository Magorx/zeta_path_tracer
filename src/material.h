#ifndef MATERIAL
#define MATERIAL

#include "ray.h"
#include "color.h"
#include "light.h"

struct HitRecord;

class Material {
	Light *emitter;
public:
	Material();

	void set_emitter(Light *emitter_);

    virtual bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const = 0;
    Vec3d emit(double surf_x, double surf_y, const Vec3d point) const;
};

#endif // MATERIAL