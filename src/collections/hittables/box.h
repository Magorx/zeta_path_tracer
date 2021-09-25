#ifndef HITTABLE_BOX_H
#define HITTABLE_BOX_H

#include "utils_header.h"

#include "rect_xy.h"
#include "rect_xz.h"
#include "rect_yz.h"

struct h_Box : public Hittable {
    Vec3d p0;
    Vec3d p1;
    HittableList sides;

    h_Box();
    h_Box(const Vec3d &p0_, const Vec3d &p1_, Material *material_);

    Vec3d normal(const Vec3d &point) const;
    bool hit(Ray &ray, HitRecord* hit_record) const override;
    bool bounding_box(AABB &box) const override;
    bool get_surface_coords(const Vec3d &point, double &sx, double &sy) const override;
};

#endif // HITTABLE_BOX_H
