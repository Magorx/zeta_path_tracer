#include "perlin.h"


#include <functional>
#include <random>


namespace zephyr::collection::noise {

Perlin::Perlin(const Vec3d &scale_)
    : scale(scale_)
{
    std::mt19937 generator;
    std::uniform_real_distribution<float> distribution;

    auto dice = std::bind(distribution, generator);
    float grad_len_2;
    for (unsigned i = 0; i < table_size; ++i) {
        do {
            gradients[i] = Vec3d(2 * dice() - 1, 2 * dice() - 1, 2 * dice() - 1);
            grad_len_2 = gradients[i].len_squared();
        } while (grad_len_2 > 1);
        gradients[i] /= sqrtf(grad_len_2);  //normalize gradient
        permutations[i] = i;
    }

    std::uniform_int_distribution<unsigned> distr_int;
    auto dice_int = std::bind(distr_int, generator);
    for (unsigned i = 0; i < table_size; ++i)
        std::swap(permutations[i], permutations[dice_int() & table_size_mask]);
    for (unsigned i = 0; i < table_size; ++i) {
        permutations[table_size + i] = permutations[i];
    }
}

int Perlin::hash(const int &x, const int &y, const int &z) const {
    return permutations[permutations[permutations[x] + y] + z];
}

double Perlin::turbulense(const Vec3d& p, int depth) const {
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++) {
        accum += weight * value(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }

    return fabs(accum);
}

inline float smoothstep(const float &t)  {
    return t * t * (3 - 2 * t);
}

template<typename T = float, typename U = float>
inline T lerp(const T &lo, const T &hi, const U &t) {
    return lo * (1 - t) + hi * t;
}

double Perlin::value(const Vec3d &p) const {
    int xi0 = ((int)floor(p.x())) & table_size_mask;
    int yi0 = ((int)floor(p.y())) & table_size_mask;
    int zi0 = ((int)floor(p.z())) & table_size_mask;

    int xi1 = (xi0 + 1) & table_size_mask;
    int yi1 = (yi0 + 1) & table_size_mask;
    int zi1 = (zi0 + 1) & table_size_mask;

    float tx = p.x() - ((int)floor(p.x()));
    float ty = p.y() - ((int)floor(p.y()));
    float tz = p.z() - ((int)floor(p.z()));

    float u = smoothstep(tx);
    float v = smoothstep(ty);
    float w = smoothstep(tz);

    // gradients at the corner of the cell
    const Vec3d &c000 = gradients[hash(xi0, yi0, zi0)];
    const Vec3d &c100 = gradients[hash(xi1, yi0, zi0)];
    const Vec3d &c010 = gradients[hash(xi0, yi1, zi0)];
    const Vec3d &c110 = gradients[hash(xi1, yi1, zi0)];

    const Vec3d &c001 = gradients[hash(xi0, yi0, zi1)];
    const Vec3d &c101 = gradients[hash(xi1, yi0, zi1)];
    const Vec3d &c011 = gradients[hash(xi0, yi1, zi1)];
    const Vec3d &c111 = gradients[hash(xi1, yi1, zi1)];

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    Vec3d p000 = Vec3d(x0, y0, z0);
    Vec3d p100 = Vec3d(x1, y0, z0);
    Vec3d p010 = Vec3d(x0, y1, z0);
    Vec3d p110 = Vec3d(x1, y1, z0);

    Vec3d p001 = Vec3d(x0, y0, z1);
    Vec3d p101 = Vec3d(x1, y0, z1);
    Vec3d p011 = Vec3d(x0, y1, z1);
    Vec3d p111 = Vec3d(x1, y1, z1);

    // linear interpolation
    float a = lerp(c000.dot(p000), c100.dot(p100), u);
    float b = lerp(c010.dot(p010), c110.dot(p110), u);
    float c = lerp(c001.dot(p001), c101.dot(p101), u);
    float d = lerp(c011.dot(p011), c111.dot(p111), u);

    float e = lerp(a, b, v);
    float f = lerp(c, d, v);

    return lerp(e, f, w) + 0.5;
}

} // namespace zephyr::collection::noise
