#ifndef HITTABLE_RECT_XY_H
#define HITTABLE_RECT_XY_H

#include "utils_header.h"

struct h_RectXY : public Hittable {
    Vec3d p0;
    Vec3d p1;

    h_RectXY();
    h_RectXY(const Vec3d &p0_, const Vec3d &p1_, Material *material_);

    Vec3d normal(const Vec3d &point) const;
    bool hit(Ray &ray, HitRecord* hit_record) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

#endif // HITTABLE_RECT_XY_H
