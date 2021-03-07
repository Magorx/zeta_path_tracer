#ifndef INTERSECTION
#define INTERSECTION

#include "vec3d.h"

class Material;

struct HitRecord {
    const Vec3d  p;
    const double dist;
    Vec3d  n;
    const Material *mat;
    bool front_hit;

    HitRecord();
    HitRecord(const Vec3d point, double distance, const Vec3d normal, Material* material, const Vec3d ray_dir);

    inline void set_normal_orientation(const Vec3d &ray_direction) {
    	front_hit = n.dot(ray_direction) < 0;
    	n = front_hit ? n : -n;
    }
};

bool operator==(const HitRecord &first, const HitRecord &second);
bool operator< (const HitRecord &first, const HitRecord &second);

extern const HitRecord HITREC_NONE;

#endif // INTERSECTION
