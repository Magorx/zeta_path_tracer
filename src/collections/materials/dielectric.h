#ifndef MATERIAL_DIELECTRIC_H
#define MATERIAL_DIELECTRIC_H

#include "utils_header.h"

class m_Dielectric : public Material {
private:
    static double reflectance(double cosine, double ref_idx);

public:
    Texture *albedo;
    double refrac_coef;
    double roughness;
    double fuzziness;
    char to_affect_emitter;

    m_Dielectric(const Color &albedo_, const double refrac_coef_ = 0, const double roughness_ = -1, const double fuzziness = 0);
    m_Dielectric(Texture *texture_,    const double refrac_coef_ = 0, const double roughness_ = -1, const double fuzziness = 0);

    bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const override;
    void affect_emitter(Vec3d &emitted_color, const double surf_x, const double surf_y, const Vec3d &point) const override;
};

#endif // MATERIAL_DIELECTRIC_H
