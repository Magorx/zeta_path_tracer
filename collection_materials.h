#ifndef COLLECTION_MATERIALS
#define COLLECTION_MATERIALS

#include "material.h"
#include "hit_record.h"

class m_Lambertian : public Material {
public:
	Color albedo;

	m_Lambertian(const Color &albedo_);

	bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const override;
};

class m_Metal : public Material {
public:
	Color albedo;
	double fuzziness;

	m_Metal(const Color &albedo_, const double fuzziness_ = 0);

	bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const override;
};

class m_Dielectric : public Material {
private:
	static double reflectance(double cosine, double ref_idx);

public:
	Color albedo;
	double refrac_coef;
	double roughness;

	m_Dielectric(const Color &albedo_, const double refrac_coef_ = 0, const double roughness_ = -1);

	bool scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const override;
};

#endif // COLLECTION_MATERIALS