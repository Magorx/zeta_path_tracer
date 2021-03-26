#ifndef RAY
#define RAY

#include "utils/vec3d.h"

using Point = Vec3d;

class Ray {
public:
    Point orig;
    Vec3d dir;

    Ray();
    Ray(const Point& origin, const Vec3d& direction);

    Point cast(double t) const;
};

#endif // RAY