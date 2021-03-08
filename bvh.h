#ifndef BVH_H
#define BVH_H

#include "hittable.h"
#include "hittable_list.h"
#include "aabb.h"
#include <algorithm>

class BVH_Node : public Hittable {
public:
	Hittable *left;
	Hittable *right;
	AABB box;

	BVH_Node();
	BVH_Node(HittableList &hitlist);
	BVH_Node(HittableList &hitlist, size_t from, size_t to);

	virtual HitRecord hit(Ray &ray) const override;
	virtual bool bounding_box(AABB &box) const override;
};



#endif // BVH_H