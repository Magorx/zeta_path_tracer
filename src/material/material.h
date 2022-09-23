#ifndef MATERIAL
#define MATERIAL

#include "utils/ray.h"
#include "color/color.h"
#include "camera_light_scene/light.h"

struct HitRecord;

class Material {
    Light *emitter;
public:
    Material();

    void set_emitter(Light *emitter_);
    Vec3d emit(const double surf_x, const double surf_y, const Vec3d &point) const;

    virtual bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const = 0;
    virtual void affect_emitter(Vec3d &emitted_color, const double surf_x, const double surf_y, const Vec3d &point) const = 0;
};

#endif // MATERIAL