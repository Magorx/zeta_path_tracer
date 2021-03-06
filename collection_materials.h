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

#endif // COLLECTION_MATERIALS