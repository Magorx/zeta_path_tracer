#include "hit_record.h"

const HitRecord HITREC_NONE({0, 0, 0}, -1, {0, 0, 0}, nullptr);

HitRecord::HitRecord(const Vec3d point, double distance, const Vec3d normal, Material* maretial) :
p   (point),
dist(distance),
n   (normal.normal()),
mat (maretial)
{}

bool operator==(const HitRecord &first, const HitRecord &second) {
    return first.p == second.p && first.dist == second.dist && first.n == second.n;
}

bool operator<(const HitRecord &first, const HitRecord &second) {
    if (first.dist == -1) {
        return false;
    } else if (second.dist == -1) {
        return true;
    } else {
        return first.dist < second.dist;
    }
}
