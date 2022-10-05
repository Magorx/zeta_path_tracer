#pragma once


#include "utils_header.h"
#include <collections/noises/perlin.h>


// poorly optimized but actually working
class t_Marble : public Texture {
private:
    Texture *first;
    Texture *second;

    zepher::collection::noise::Perlin noise;

public:
    t_Marble(Texture *first_=nullptr, Texture *second_=nullptr, const Vec3d &scale_=1);
    t_Marble(const Color &first_, const Color &second_, const Vec3d &scale_=1);

    virtual Color value(double sx, double sy, const Vec3d &p) const override;
};