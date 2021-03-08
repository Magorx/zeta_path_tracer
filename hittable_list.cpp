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

bool HittableList::bounding_box(AABB &output_box) const {
	if (hittables.empty()) {
		return false;
	}

	AABB tmp_box;
	bool first_box = true;

	for (const Hittable *obj : hittables) {
		if (!obj->bounding_box(tmp_box)) return false;
        output_box = first_box ? tmp_box : surrounding_box(output_box, tmp_box);
        first_box = false;
	}

	return true;
}

size_t HittableList::size() const {
	return hittables.size();
}

Hittable *HittableList::operator[](const size_t i) const {
	return hittables[i];
}