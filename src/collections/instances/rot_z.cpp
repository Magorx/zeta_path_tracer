#include "rot_z.h"


inst_RotZ::inst_RotZ(Hittable *obj_, const double angle): inst_RotZ(obj_, sin(angle), cos(angle)) {}
inst_RotZ::inst_RotZ(Hittable *obj_, const double sin_a, const double cos_a):
Instance(obj_),
cos_a(cos_a),
sin_a(sin_a),
has_bbox(false),
bbox()
{
    has_bbox = obj->bounding_box(bbox);

    Vec3d mn( VEC3D_INF,  VEC3D_INF,  VEC3D_INF);
    Vec3d mx(-VEC3D_INF, -VEC3D_INF, -VEC3D_INF);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                const Vec3d p(i, j, k);
                const auto mixed = p * bbox.mx + (1 - p) * bbox.mn;
                const double x = mixed[0];
                const double y = mixed[1];
                const double z = mixed[2];

                double newx =  cos_a*x + sin_a*y;
                double newy = -sin_a*x + cos_a*y;

                Vec3d cur_vec(newx, newy, z);

                for (int c = 0; c < 3; c++) {
                    mn.set(c, fmin(mn[c], cur_vec[c]));
                    mx.set(c,  fmax(mx[c], cur_vec[c]));
                }
            }
        }
    }

    bbox = AABB(mn, mx);
}

bool inst_RotZ::hit(Ray &ray, HitRecord* hit_record) const {
	Vec3d orig = ray.orig;
    Vec3d dir  = ray.dir;

    orig.set(0, cos_a*ray.orig[0] - sin_a*ray.orig[1]);
    orig.set(1, sin_a*ray.orig[0] + cos_a*ray.orig[1]);

    dir.set(0, cos_a*ray.dir[0] - sin_a*ray.dir[1]);
    dir.set(1, sin_a*ray.dir[0] + cos_a*ray.dir[1]);

    Ray rot_ray(orig, dir);

    if (!obj->hit(rot_ray, hit_record)) {
        return false;
    }

    Vec3d p = hit_record->point;
    Vec3d normal = hit_record->normal;

    p.set(0,  cos_a*hit_record->point[0] + sin_a * hit_record->point[1]);
    p.set(1, -sin_a*hit_record->point[0] + cos_a * hit_record->point[1]);

    normal.set(0,  cos_a*hit_record->normal[0] + sin_a * hit_record->normal[1]);
    normal.set(1, -sin_a*hit_record->normal[0] + cos_a * hit_record->normal[1]);

    hit_record->point = p;
    hit_record->normal = normal;
    hit_record->set_normal_orientation(ray.dir);
    // fprintf(stderr, "dist %lg\normal", hitrec.dist);

    return true;
}

bool inst_RotZ::bounding_box(AABB &box) const {
	box = bbox;
	return has_bbox;
}

bool inst_RotZ::get_surface_coords(const Vec3d&, double&, double&) const {
	return false;
}
