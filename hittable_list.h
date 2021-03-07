#ifndef HITTABLE_LIST
#define HITTABLE_LIST

#include "hittable.h"
#include "hit_record.h"
#include <vector>

class HittableList : public Hittable {
	std::vector<Hittable*> hittables;

public:
	HittableList();

	void insert(Hittable *hittable);

	HitRecord hit(Ray &ray) const override;
};


#endif // HITTABLE_LIST