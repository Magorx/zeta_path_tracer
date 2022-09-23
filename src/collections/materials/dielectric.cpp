#include "dielectric.h"

m_Dielectric::m_Dielectric(const Color &albedo_, const double refrac_coef_, const double roughness_, const double fuzziness):
Material(),
albedo(new t_SolidColor(albedo_)),
refrac_coef(refrac_coef_),
roughness(roughness_),
fuzziness(fuzziness),
to_affect_emitter(1)
{}

m_Dielectric::m_Dielectric(Texture *texture_,    const double refrac_coef_, const double roughness_, const double fuzziness):
Material(),
albedo(texture_),
refrac_coef(refrac_coef_),
roughness(roughness_),
fuzziness(fuzziness),
to_affect_emitter(1)
{}

bool m_Dielectric::scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const {
    attenuation = albedo->value(hitrec.surf_x, hitrec.surf_y, hitrec.point);
    double rc = hitrec.front_hit ? (1 / refrac_coef) : refrac_coef;

    double cos_theta = fmin(-(ray.dir.dot(hitrec.normal)) / ray.dir.len(), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = rc * sin_theta > 1.0;
    Vec3d scatter_direction;

    unsigned int random_values[4];
    Brans::rand_sse(random_values);
    double first_random_value  = (double) random_values[0] / (double) UINT32_MAX;
    double second_random_value = (double) random_values[1] / (double) UINT32_MAX;

    if (cannot_refract || reflectance(cos_theta, rc) > first_random_value || (roughness > 0 && second_random_value < roughness)) {
        scatter_direction = reflect(ray.dir, -hitrec.normal);
    } else {
        scatter_direction = refract(ray.dir, hitrec.normal, rc);
    }

    Vec3d fuzz_scatter = Vec3d::random_in_unit_sphere();
    fuzz_scatter *= fuzziness;
    scatter_direction += fuzz_scatter;

    scattered = Ray(hitrec.point, scatter_direction);

    return true;
}

double m_Dielectric::reflectance(double cosine, double ref_idx) { // Schlick Approximation from RayTracingInOneWeekend
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*pow((1 - cosine), 5);
}

void m_Dielectric::affect_emitter(Vec3d &, const double, const double, const Vec3d&) const {}
