#include "rot_y.h"


inst_RotY::inst_RotY(Hittable *obj_, const double angle): inst_RotY(obj_, sin(angle), cos(angle)) {}
inst_RotY::inst_RotY(Hittable *obj_, const double sin_a, const double cos_a):
obj(obj_),
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

                double newx =  cos_a*x + sin_a*z;
                double newz = -sin_a*x + cos_a*z;

                Vec3d cur_vec(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    mn.set(c, fmin(mn[c], cur_vec[c]));
                    mx.set(c, fmax(mx[c], cur_vec[c]));
                }
            }
        }
    }

    bbox = AABB(mn, mx);
    // fprintf(stderr, "bbox [%lg %lg %lg] [%lg %lg %lg]\normal", bbox.mn.x, bbox.mn.y, bbox.mn.z, bbox.mx.x, bbox.mx.y, bbox.mx.z);
}

bool inst_RotY::hit(Ray &ray, HitRecord* hit_record) const {
	Vec3d orig = ray.orig;
    Vec3d dir  = ray.dir;

    orig.set(0, cos_a*ray.orig[0] - sin_a*ray.orig[2]);
    orig.set(2, sin_a*ray.orig[0] + cos_a*ray.orig[2]);

    dir.set(0, cos_a*ray.dir[0] - sin_a*ray.dir[2]);
    dir.set(2, sin_a*ray.dir[0] + cos_a*ray.dir[2]);

    Ray rot_ray(orig, dir);

    if (!obj->hit(rot_ray, hit_record)) {
        return false;
    }

    Vec3d p = hit_record->point;
    Vec3d normal = hit_record->normal;

    p.set(0,  cos_a*hit_record->point[0] + sin_a * hit_record->point[2]);
    p.set(2, -sin_a*hit_record->point[0] + cos_a * hit_record->point[2]);

    normal.set(0, cos_a*hit_record->normal[0] + sin_a * hit_record->normal[2]);
    normal.set(2, -sin_a*hit_record->normal[0] + cos_a * hit_record->normal[2]);

    hit_record->point = p;
    hit_record->normal = normal;
    hit_record->set_normal_orientation(rot_ray.dir);
    // fprintf(stderr, "dist %lg\normal", hitrec.dist);

    return true;
}

bool inst_RotY::bounding_box(AABB &box) const {
	box = bbox;
	return has_bbox;
}

bool inst_RotY::get_surface_coords(const Vec3d&, double&, double&) const {
	return false;
}
