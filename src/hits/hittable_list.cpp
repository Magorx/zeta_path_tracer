#include <acceleration_structures/bvh.h>
#include "hittable_list.h"

HittableList::HittableList()
{}

void HittableList::insert(Hittable *hittable) {
	hittables.push_back(hittable);
}

bool HittableList::hit(Ray &ray, HitRecord* hit_record) const {
    bool hit_occurred = false;
    for (auto hittable : hittables) {
		if(hittable->hit(ray, hit_record)) hit_occurred = true;
	}
    return hit_occurred;
    //return true;

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

bool HittableList::bounding_box(AABB &output_box, size_t from, size_t to) const {
	if (hittables.empty()) {
		return false;
	}

	AABB tmp_box;
	bool first_box = true;

	from = std::max(std::min(from, hittables.size()), 0ul);
	to   = std::max(std::min(to,   hittables.size()), 0ul);
	for (size_t i = from; i < to; ++i) {
		Hittable *obj = hittables[i];
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



Hittable* HittableList::get_bvh_tree() {
    if(hittables.size() <= 4) {
        HittableList* new_tree = new HittableList();
        for(auto* children : hittables) {
            new_tree->insert(children->get_bvh_tree());
        }
        return new_tree;
    }

    return new BVH_Node(*this, 0, hittables.size());
}
