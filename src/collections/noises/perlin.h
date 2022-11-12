#pragma once


#include <utils/vec3d.h>


namespace zephyr::collection::noise {

class Perlin {
    Vec3d scale;

    static const size_t table_size = 256;
    static const size_t table_size_mask = table_size - 1;

    Vec3d gradients[table_size];
    size_t permutations[table_size * 2];

public:
    Perlin(const Vec3d &scale_=1);

    double value     (const Vec3d &p)              const;
    double turbulense(const Vec3d& p, int depth=7) const;

    int hash(const int &x, const int &y, const int &z) const;

    inline Vec3d get_scale() const {
        return scale;
    }
};

}; // namespace zephyr::collection::noise
