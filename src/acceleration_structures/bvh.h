#ifndef BVH_H
#define BVH_H

#include "hits/hittable.h"
#include "hits/hittable_list.h"
#include "hits/aabb.h"

#include <cstdio>
#include <algorithm>

class BVH_Node : public Hittable {
public:
    Hittable *left;
    Hittable *right;
    AABB box;

    BVH_Node();
    BVH_Node(HittableList &hitlist);
    BVH_Node(HittableList &hitlist, size_t from, size_t to);

    virtual bool hit(Ray &ray, HitRecord* hit_record) const override;
    virtual bool bounding_box(AABB &box) const override;

    void dump(int depth = 0);

    virtual void dump_bvh(int depth) override;
};

double BVH_Node_by_axis_estimation(HittableList &hitlist, size_t from, size_t to, const int axis);


#endif // BVH_H