#ifndef TEXTURE_CHECKERED_H
#define TEXTURE_CHECKERED_H

#include "utils_header.h"

class t_Checkered : public Texture {
private:
    Texture *first;
    Texture *second;
    Vec3d freq;

public:
    t_Checkered();
    t_Checkered(const Color &first_, const Color &second_ = VEC3D_ZERO, const Vec3d &freq_ = VEC3D_ONE);
    t_Checkered(Texture *first_, Texture *second_, const Vec3d &freq_);

    virtual Color value(double sx, double sy, const Vec3d &p) const override;
};

#endif // TEXTURE_CHECKERED_H
