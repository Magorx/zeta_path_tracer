#include "hittable_list.h"

HittableList::HittableList()
{}

void HittableList::insert(Hittable *hittable) {
	hittables.push_back(hittable);
}

HitRecord HittableList::hit(Ray &ray) const {
	HitRecord hitrec = HITREC_NONE;

	for (auto hittable : hittables) {
		HitRecord hr = hittable->hit(ray);
		if (hitrec.dist < 0 || (hr.dist > 0 && hr.dist < hitrec.dist)) {
			hitrec = hr;
		}
	}

	return hitrec;
}