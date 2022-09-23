#ifndef LIGHT_DIFFUSE_H
#define LIGHT_DIFFUSE_H

#include "utils_header.h"

class l_Diffuse : public Light {
public:
    Color color;
    l_Diffuse(const Color color_);
    Vec3d emit(double, double, const Vec3d&) const override;
};

#endif // LIGHT_DIFFUSE_H
