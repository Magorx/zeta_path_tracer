#include "ray.h"

Ray::Ray() {}

Ray::Ray(const Point& origin, const Vec3d& direction):
orig(origin),
dir(direction)
{
    dir.normalize();
}