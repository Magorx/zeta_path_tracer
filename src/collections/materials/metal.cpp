#include "metal.h"

m_Metal::m_Metal(const Color &albedo_, const double fuzziness_):
Material(),
albedo(new t_SolidColor(albedo_)),
fuzziness(fuzziness_),
to_affect_emitter(1)
{}

m_Metal::m_Metal(Texture *texture_, const double fuzziness_):
Material(),
albedo(texture_),
fuzziness(fuzziness_),
to_affect_emitter(1)
{}

bool m_Metal::scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const {
    Vec3d scatter_direction = Vec3d::random_in_unit_sphere();
    scatter_direction *= fuzziness;
    scatter_direction += reflect(ray.dir, hitrec.normal);

    scattered = Ray(hitrec.point, scatter_direction);
    attenuation = albedo->value(hitrec.surf_x, hitrec.surf_y, hitrec.point);
    return true;
}

void m_Metal::affect_emitter(Vec3d &, const double, const double, const Vec3d&) const {}
