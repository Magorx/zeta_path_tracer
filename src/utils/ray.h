#pragma once

#include "utils/vec3d.h"

using Point = Vec3d;

class Ray {
public:
    Point orig;
    Vec3d dir;

    Ray();
    Ray(const Point& origin, const Vec3d& direction);

    inline Point cast(double t) const {
        Vec3d result = dir;
        result *= t;
        result += orig;
        return result;
    }
};
