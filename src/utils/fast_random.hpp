#ifndef FAST_RANDOM_H
#define FAST_RANDOM_H

#include <ctime>
#include <random>
#include <type_traits>

// A function to return a seeded random number generator.
inline std::mt19937& generator() {
    // the generator will only be seeded once (per thread) since it's static
    static thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}

// A function to generate integers in the range [min, max]
template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
T my_rand(T min, T max) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(generator());
}

// A function to generate floats in the range [min, max)
template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
T my_rand(T min, T max) {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(generator());
}

namespace Brans {

// uncoment this if you are using intel compiler
// for MS CL the vectorizer is on by default and jumps in if you
// compile with /O2 ...
//#pragma intel optimization_parameter target_arch=avx
//__declspec(cpu_dispatch(core_2nd_gen_avx, core_i7_sse4_2, core_2_duo_ssse3, generic )
inline void rand_sse(unsigned int* result) {
        result[0] = my_rand<int>(INT_MIN, INT_MAX);
        result[1] = my_rand<int>(INT_MIN, INT_MAX);
        result[2] = my_rand<int>(INT_MIN, INT_MAX);
        result[3] = my_rand<int>(INT_MIN, INT_MAX);
}

inline void srand_sse(unsigned int seed) {
    srand(seed);
}


};

#endif // FAST_RANDOM_H