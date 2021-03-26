#ifndef COLLECTION_MATERIALS
#define COLLECTION_MATERIALS

#include "material/material.h"
#include "hits/hit_record.h"
#include "color/texture.h"

#include "collection_textures.h"

class m_Lambertian : public Material {
public:
	Texture *albedo;
	char to_affect_emitter;

	m_Lambertian(const Color &albedo_);
	m_Lambertian(Texture *texture_);

	bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const override;
	void affect_emitter(Vec3d &emitted_color, const double surf_x, const double surf_y, const Vec3d &point) const override;
};

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

class m_Dielectric : public Material {
private:
	static double reflectance(double cosine, double ref_idx);

public:
	Texture *albedo;
	double refrac_coef;
	double roughness;
	char to_affect_emitter;

	m_Dielectric(const Color &albedo_, const double refrac_coef_ = 0, const double roughness_ = -1);
	m_Dielectric(Texture *texture_,    const double refrac_coef_ = 0, const double roughness_ = -1);

	bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const override;
	void affect_emitter(Vec3d &emitted_color, const double surf_x, const double surf_y, const Vec3d &point) const override;
};

#endif // COLLECTION_MATERIALS