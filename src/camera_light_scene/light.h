#ifndef LIGHT
#define LIGHT

#include "utils/vec3d.h"

class Light {
public:
    virtual Vec3d emit(double surf_x, double surf_y, const Vec3d &point) const = 0;
};

#endif // LIGHT