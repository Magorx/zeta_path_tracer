#ifndef INTERSECTION
#define INTERSECTION

#include "utils/vec3d.h"

class Material;

struct HitRecord {
    Vec3d  point;
    double dist;
    Vec3d  normal;
    Material *mat;
    bool front_hit;

    double surf_x;
    double surf_y;

    HitRecord();
    HitRecord(const Vec3d& point, double distance, const Vec3d& normal, Material* material, const Vec3d& ray_dir);

    inline void set_normal_orientation(const Vec3d &ray_direction) {
    	front_hit = normal.dot(ray_direction) < 0;
        normal *= front_hit * 2 - 1;
    }
};

bool operator==(const HitRecord &first, const HitRecord &second);
bool operator< (const HitRecord &first, const HitRecord &second);

extern const HitRecord HITREC_NONE;

#endif // INTERSECTION
