#ifndef TEXTURE_SOLID_COLOR_H
#define TEXTURE_SOLID_COLOR_H

#include "utils_header.h"

class t_SolidColor : public Texture {
private:
    Color color;

public:
    t_SolidColor();
    t_SolidColor(const Color &color_);

    virtual Color value(double sx, double sy, const Vec3d &p) const override;
};

#endif // TEXTURE_SOLID_COLOR_H
