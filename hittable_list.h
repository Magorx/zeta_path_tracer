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

	HitRecord hit(Ray &ray) const override;
	bool bounding_box(AABB &box) const override;

	size_t size() const;
	Hittable *operator[](const size_t i) const;

};


#endif // HITTABLE_LIST