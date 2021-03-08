#ifndef AABB_H
#define AABB_H

#include "vec3d.h"
#include "ray.h"

class AABB {
public:
	Vec3d mn;
	Vec3d mx;

	AABB();
	AABB(const Vec3d minimum, const Vec3d maximum);

	bool hit(const Ray &ray, double t_min, double t_max) const;

	double effective_size() const;
};

AABB surrounding_box(const AABB first, const AABB second);

#endif // AABB_H