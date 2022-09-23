#include "translate.h"

inst_Translate::inst_Translate(Hittable *obj_, const Vec3d &offset_):
Instance(obj_),
offset(offset_)
{}

bool inst_Translate::hit(Ray &ray, HitRecord* hit_record) const {
    Ray moved_r(ray.orig - offset, ray.dir);

    if (!obj->hit(moved_r, hit_record)) {
        return false;
    }

    hit_record->point += offset;
    hit_record->set_normal_orientation(moved_r.dir);

    return true;
}

bool inst_Translate::bounding_box(AABB &box) const {
    if (!obj->bounding_box(box))
        return false;

    box = AABB(box.mn + offset, box.mx + offset);
    return true;
}

bool inst_Translate::get_surface_coords(const Vec3d&, double&, double&) const {
    return false;
}
