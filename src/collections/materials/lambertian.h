#ifndef MATERIAL_LAMBERTIAN_H
#define MATERIAL_LAMBERTIAN_H

#include "utils_header.h"

class m_Lambertian : public Material {
public:
    Texture *albedo;
    char to_affect_emitter;

    m_Lambertian(const Color &albedo_);
    m_Lambertian(Texture *texture_);

    bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const override;
    void affect_emitter(Vec3d &emitted_color, const double surf_x, const double surf_y, const Vec3d &point) const override;
};


#endif // MATERIAL_LAMBERTIAN_H
