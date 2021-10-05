#ifndef INSTANCE_ROT_Z_H
#define INSTANCE_ROT_Z_H

#include "utils_header.h"

class inst_RotZ : public Instance {
public:
	double cos_a;
	double sin_a;
	bool has_bbox;
	AABB bbox;

	inst_RotZ(Hittable *obj_, const double angle);
    inst_RotZ(Hittable *obj_, const double sin_a, const double cos_a);
	virtual bool hit(Ray &ray, HitRecord* hit_record) const override;
	virtual bool bounding_box(AABB &box) const override;
	virtual bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;

    Hittable* get_bvh_tree() override { return new inst_RotZ(obj->get_bvh_tree(), sin_a, cos_a); }
};

#endif // INSTANCE_ROT_Z_H
