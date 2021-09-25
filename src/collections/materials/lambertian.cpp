#include "lambertian.h"

m_Lambertian::m_Lambertian(const Color &albedo_):
Material(),
albedo(new t_SolidColor(albedo_)),
to_affect_emitter(1)
{}

m_Lambertian::m_Lambertian(Texture *texture_):
Material(),
albedo(texture_),
to_affect_emitter(1)
{}

bool m_Lambertian::scatter(const Ray &, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const {
	Vec3d scatter_direction = hitrec.normal;
    scatter_direction += Vec3d::random_unit(); // TODO plz
	if (scatter_direction.is_zero()) {
		scatter_direction = hitrec.normal;
	}

    scattered = Ray(hitrec.point, scatter_direction);
    attenuation = albedo->value(hitrec.surf_x, hitrec.surf_y, hitrec.point);
    return true;
}

void m_Lambertian::affect_emitter(Vec3d &emitted, const double sx, const double sy, const Vec3d &point) const {
	emitted *= albedo->value(sx, sy, point) / d_MAXRGB;
}
