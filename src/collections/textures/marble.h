#pragma once


#include "utils_header.h"

// poorly optimized but actually working
class t_Marble : public Texture {
private:
    Texture *first;
    Texture *second;
    Vec3d freq;
    double scale;

    static const size_t table_size = 256;
    static const size_t table_size_mask = table_size - 1;

    Vec3d gradients[table_size];
    size_t permutations[table_size * 2];

    double perlin(const Vec3d &p) const;
    int hash(const int &x, const int &y, const int &z) const;

    double turb(const Vec3d& p, int depth=7) const;

public:
    t_Marble(Texture *first_=nullptr, Texture *second_=nullptr, const Vec3d &freq_=VEC3D_ONE, double scale_=1);
    t_Marble(const Color &first_, const Color &second_, const Vec3d &freq_, double scale_);

    virtual Color value(double sx, double sy, const Vec3d &p) const override;
};