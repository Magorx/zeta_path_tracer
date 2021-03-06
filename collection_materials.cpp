#include "collection_materials.h"

m_Lambertian::m_Lambertian(const Color &albedo_):
albedo(albedo_)
{}

bool m_Lambertian::scatter(const Ray &, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const {
    scattered = Ray(hitrec.p, hitrec.n + Vec3d::random_unit());
    attenuation = albedo;
    return true;
}
