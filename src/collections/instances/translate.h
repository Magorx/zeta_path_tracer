#ifndef INSTANCE_TRANSLATE_H
#define INSTANCE_TRANSLATE_H

#include "utils_header.h"

class inst_Translate : public Hittable {
public:
	Hittable *obj;
	Vec3d offset;

	inst_Translate(Hittable *obj_, const Vec3d &offset_);
	virtual bool hit(Ray &ray, HitRecord* hit_record) const override;
	virtual bool bounding_box(AABB &box) const override;
	virtual bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;

    Hittable* get_bvh_tree() override { return new inst_Translate(obj->get_bvh_tree(), offset); }
};

#endif // INSTANCE_TRANSLATE_H
