#ifndef COLLECTION_INSTANCES
#define COLLECTION_INSTANCES

#include "hits/hittable.h"

class inst_Translate : public Hittable {
public:
	Hittable *obj;
	Vec3d offset;

	inst_Translate(Hittable *obj_, const Vec3d &offset_);
	virtual HitRecord hit(Ray &ray) const override;
	virtual bool bounding_box(AABB &box) const override;
	virtual bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

class inst_RotX : public Hittable {
public:
	Hittable *obj;
	double cos_a;
	double sin_a;
	bool has_bbox;
	AABB bbox;

	inst_RotX(Hittable *obj_, const double angle);
	virtual HitRecord hit(Ray &ray) const override;
	virtual bool bounding_box(AABB &box) const override;
	virtual bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

class inst_RotY : public Hittable {
public:
	Hittable *obj;
	double cos_a;
	double sin_a;
	bool has_bbox;
	AABB bbox;

	inst_RotY(Hittable *obj_, const double angle);
	virtual HitRecord hit(Ray &ray) const override;
	virtual bool bounding_box(AABB &box) const override;
	virtual bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

class inst_RotZ : public Hittable {
public:
	Hittable *obj;
	double cos_a;
	double sin_a;
	bool has_bbox;
	AABB bbox;

	inst_RotZ(Hittable *obj_, const double angle);
	virtual HitRecord hit(Ray &ray) const override;
	virtual bool bounding_box(AABB &box) const override;
	virtual bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

#endif // COLLECTION_INSTANCES