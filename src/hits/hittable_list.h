#ifndef HITTABLE_LIST
#define HITTABLE_LIST

#include "hittable.h"
#include "hit_record.h"
#include "aabb.h"
#include <vector>

class HittableList : public Hittable {
public:
	std::vector<Hittable*> hittables;

	HittableList();

	void insert(Hittable *hittable);

	bool hit(Ray &ray, HitRecord* hit_record) const override;
	bool bounding_box(AABB &output_box) const override;
	bool bounding_box(AABB &output_box, size_t from, size_t to) const;

	size_t size() const;
	Hittable *operator[](const size_t i) const;

    Hittable* get_bvh_tree() override;
};


#endif // HITTABLE_LIST