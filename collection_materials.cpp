#include "collection_materials.h"

m_Lambertian::m_Lambertian(const Color &albedo_):
albedo(albedo_)
{}

bool m_Lambertian::scatter(const Ray &, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const {
	Vec3d scatter_direction = hitrec.n + Vec3d::random_unit();
	if (scatter_direction.is_zero()) {
		scatter_direction = hitrec.n;
	}

    scattered = Ray(hitrec.p, scatter_direction);
    attenuation = albedo;
    return true;
}
