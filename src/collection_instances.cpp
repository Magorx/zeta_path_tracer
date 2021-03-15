#include "collection_instances.h"

inst_Translate::inst_Translate(Hittable *obj_, const Vec3d &offset_):
obj(obj_),
offset(offset_)
{}

HitRecord inst_Translate::hit(Ray &ray) const {
	Ray moved_r(ray.orig - offset, ray.dir);
	HitRecord hitrec;
    if ((hitrec = obj->hit(moved_r)).dist < 0)
        return hitrec;

    hitrec.p += offset;
    hitrec.set_normal_orientation(moved_r.dir);

    return hitrec;
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

//=============================================================================

inst_RotZ::inst_RotZ(Hittable *obj_, const double angle):
obj(obj_),
cos_a(1),
sin_a(0),
has_bbox(false),
bbox()
{
    sin_a = sin(angle);
    cos_a = cos(angle);
    has_bbox = obj->bounding_box(bbox);

    Vec3d mn( VEC3D_INF,  VEC3D_INF,  VEC3D_INF);
    Vec3d mx(-VEC3D_INF, -VEC3D_INF, -VEC3D_INF);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                double x = i*bbox.mx.x + (1-i)*bbox.mn.x;
                double y = j*bbox.mx.y + (1-j)*bbox.mn.y;
                double z = k*bbox.mx.z + (1-k)*bbox.mn.z;

                double newx =  cos_a*x + sin_a*y;
                double newy = -sin_a*x + cos_a*y;

                Vec3d cur_vec(newx, newy, z);

                for (int c = 0; c < 3; c++) {
                    mn[c] = fmin(mn[c], cur_vec[c]);
                    mx[c] = fmax(mx[c], cur_vec[c]);
                }
            }
        }
    }

    bbox = AABB(mn, mx);
    // fprintf(stderr, "bbox [%lg %lg %lg] [%lg %lg %lg]\n", bbox.mn.x, bbox.mn.y, bbox.mn.z, bbox.mx.x, bbox.mx.y, bbox.mx.z);
}

HitRecord inst_RotZ::hit(Ray &ray) const {
	Vec3d orig = ray.orig;
    Vec3d dir  = ray.dir;

    orig[0] = cos_a*ray.orig[0] - sin_a*ray.orig[1];
    orig[1] = sin_a*ray.orig[0] + cos_a*ray.orig[1];

    dir[0] = cos_a*ray.dir[0] - sin_a*ray.dir[1];
    dir[1] = sin_a*ray.dir[0] + cos_a*ray.dir[1];

    Ray rot_ray(orig, dir);

    HitRecord hitrec;
    if ((hitrec = obj->hit(rot_ray)).dist < 0) {
        return hitrec;
    }

    Vec3d p = hitrec.p;
    Vec3d normal = hitrec.n;

    p[0] =  cos_a*hitrec.p[0] + sin_a*hitrec.p[1];
    p[1] = -sin_a*hitrec.p[0] + cos_a*hitrec.p[1];

    normal[0] =  cos_a*hitrec.n[0] + sin_a*hitrec.n[1];
    normal[1] = -sin_a*hitrec.n[0] + cos_a*hitrec.n[1];

    hitrec.p = p;
    hitrec.set_normal_orientation(rot_ray.dir);
    // fprintf(stderr, "dist %lg\n", hitrec.dist);

    return hitrec;
}

bool inst_RotZ::bounding_box(AABB &box) const {
	box = bbox;
	return has_bbox;
}

bool inst_RotZ::get_surface_coords(const Vec3d&, double&, double&) const {
	return false;
}

//=============================================================================

inst_RotX::inst_RotX(Hittable *obj_, const double angle):
obj(obj_),
cos_a(1),
sin_a(0),
has_bbox(false),
bbox()
{
    sin_a = sin(angle);
    cos_a = cos(angle);
    has_bbox = obj->bounding_box(bbox);

    Vec3d mn( VEC3D_INF,  VEC3D_INF,  VEC3D_INF);
    Vec3d mx(-VEC3D_INF, -VEC3D_INF, -VEC3D_INF);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                double x = i*bbox.mx.x + (1-i)*bbox.mn.x;
                double y = j*bbox.mx.y + (1-j)*bbox.mn.y;
                double z = k*bbox.mx.z + (1-k)*bbox.mn.z;

                double newy =  cos_a*y + sin_a*z;
                double newz = -sin_a*y + cos_a*z;

                Vec3d cur_vec(x, newy, newz);

                for (int c = 0; c < 3; c++) {
                    mn[c] = fmin(mn[c], cur_vec[c]);
                    mx[c] = fmax(mx[c], cur_vec[c]);
                }
            }
        }
    }

    bbox = AABB(mn, mx);
    // fprintf(stderr, "bbox [%lg %lg %lg] [%lg %lg %lg]\n", bbox.mn.x, bbox.mn.y, bbox.mn.z, bbox.mx.x, bbox.mx.y, bbox.mx.z);
}

HitRecord inst_RotX::hit(Ray &ray) const {
	Vec3d orig = ray.orig;
    Vec3d dir  = ray.dir;

    orig[1] = cos_a*ray.orig[1] - sin_a*ray.orig[2];
    orig[2] = sin_a*ray.orig[1] + cos_a*ray.orig[2];

    dir[1] = cos_a*ray.dir[1] - sin_a*ray.dir[2];
    dir[2] = sin_a*ray.dir[1] + cos_a*ray.dir[2];

    Ray rot_ray(orig, dir);

    HitRecord hitrec;
    if ((hitrec = obj->hit(rot_ray)).dist < 1) {
        return hitrec;
    }

    Vec3d p = hitrec.p;
    Vec3d normal = hitrec.n;

    p[1] =  cos_a*hitrec.p[1] + sin_a*hitrec.p[2];
    p[2] = -sin_a*hitrec.p[1] + cos_a*hitrec.p[2];

    normal[1] =  cos_a*hitrec.n[1] + sin_a*hitrec.n[2];
    normal[2] = -sin_a*hitrec.n[1] + cos_a*hitrec.n[2];

    hitrec.p = p;
    hitrec.set_normal_orientation(rot_ray.dir);
    // fprintf(stderr, "dist %lg\n", hitrec.dist);

    return hitrec;
}

bool inst_RotX::bounding_box(AABB &box) const {
	box = bbox;
	return has_bbox;
}

bool inst_RotX::get_surface_coords(const Vec3d&, double&, double&) const {
	return false;
}

//=============================================================================

inst_RotY::inst_RotY(Hittable *obj_, const double angle):
obj(obj_),
cos_a(1),
sin_a(0),
has_bbox(false),
bbox()
{
    sin_a = sin(angle);
    cos_a = cos(angle);
    has_bbox = obj->bounding_box(bbox);

    Vec3d mn( VEC3D_INF,  VEC3D_INF,  VEC3D_INF);
    Vec3d mx(-VEC3D_INF, -VEC3D_INF, -VEC3D_INF);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                double x = i*bbox.mx.x + (1-i)*bbox.mn.x;
                double y = j*bbox.mx.y + (1-j)*bbox.mn.y;
                double z = k*bbox.mx.z + (1-k)*bbox.mn.z;

                double newx =  cos_a*x + sin_a*z;
                double newz = -sin_a*x + cos_a*z;

                Vec3d cur_vec(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    mn[c] = fmin(mn[c], cur_vec[c]);
                    mx[c] = fmax(mx[c], cur_vec[c]);
                }
            }
        }
    }

    bbox = AABB(mn, mx);
    // fprintf(stderr, "bbox [%lg %lg %lg] [%lg %lg %lg]\n", bbox.mn.x, bbox.mn.y, bbox.mn.z, bbox.mx.x, bbox.mx.y, bbox.mx.z);
}

HitRecord inst_RotY::hit(Ray &ray) const {
	Vec3d orig = ray.orig;
    Vec3d dir  = ray.dir;

    orig[0] = cos_a*ray.orig[0] - sin_a*ray.orig[2];
    orig[2] = sin_a*ray.orig[0] + cos_a*ray.orig[2];

    dir[0] = cos_a*ray.dir[0] - sin_a*ray.dir[2];
    dir[2] = sin_a*ray.dir[0] + cos_a*ray.dir[2];

    Ray rot_ray(orig, dir);

    HitRecord hitrec;
    if ((hitrec = obj->hit(rot_ray)).dist < 0) {
        return hitrec;
    }

    Vec3d p = hitrec.p;
    Vec3d normal = hitrec.n;

    p[0] =  cos_a*hitrec.p[0] + sin_a*hitrec.p[2];
    p[2] = -sin_a*hitrec.p[0] + cos_a*hitrec.p[2];

    normal[0] =  cos_a*hitrec.n[0] + sin_a*hitrec.n[2];
    normal[2] = -sin_a*hitrec.n[0] + cos_a*hitrec.n[2];

    hitrec.p = p;
    hitrec.set_normal_orientation(rot_ray.dir);
    // fprintf(stderr, "dist %lg\n", hitrec.dist);

    return hitrec;
}

bool inst_RotY::bounding_box(AABB &box) const {
	box = bbox;
	return has_bbox;
}

bool inst_RotY::get_surface_coords(const Vec3d&, double&, double&) const {
	return false;
}
