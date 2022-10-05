#pragma once


#include "utils_header.h"
#include <collections/noises/perlin.h>


// poorly optimized but actually working
class m_Marble : public Material {
private:
    Material *first;
    Material *second;

    zepher::collection::noise::Perlin noise;

    double get_noise(const Vec3d &point) const;

public:
    m_Marble(Material *first_=nullptr, Material *second_=nullptr, const Vec3d &scale_=1);

    virtual Vec3d emit(const double surf_x, const double surf_y, const Vec3d &point) const override;
    bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const override;
    void affect_emitter(Vec3d &emitted_color, const double surf_x, const double surf_y, const Vec3d &point) const override;
};