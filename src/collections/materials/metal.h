#ifndef MATERIAL_METAL_H
#define MATERIAL_METAL_H

#include "utils_header.h"

class m_Metal : public Material {
public:
    Texture *albedo;
    double fuzziness;
    char to_affect_emitter;

    m_Metal(const Color &albedo_, const double fuzziness_ = 0);
    m_Metal(Texture *texture_,    const double fuzziness_ = 0);

    bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const override;
    void affect_emitter(Vec3d &emitted_color, const double surf_x, const double surf_y, const Vec3d &point) const override;
};

#endif // MATERIAL_METAL_H
