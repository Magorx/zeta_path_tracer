#ifndef GENERAL_VEC3D
#define GENERAL_VEC3D

#include <cmath>
#include <iostream>

const double VEC3_EPS = 1e-6;
const double VEC3_MAX_RANDOM_TRIES_CNT = 50;
const double VEC3D_INF = 1e9;
const double VEC3D_PI = 3.1415926535897;

/**
 * Vectorization by AlexRoar (https://github.com/AlexRoar).
 * Debug and inlining help by JakMobius (https://github.com/JakMobius).
 */

//=============================================================================
//<KCTF> Random for vector ====================================================

long vec3d_randlong();

double vec3d_randdouble(double dmin = 0.0, double dmax = 1.0);

//=============================================================================
//<KCTF> Vec2_header ==========================================================

struct Vec3d {
    typedef double content3 __attribute__((ext_vector_type(4)));

    content3 content;

    Vec3d();

    Vec3d(content3 newContent);

    Vec3d(double x_, double y_, double z_);

    inline double len() const {
        return sqrt(len_squared());
    }

    inline double len_squared() const {
        const auto squared = content * content;
        return squared[0] + squared[1] + squared[2];
    }

    inline Vec3d normal() const {
        double l = len();
        if(l < VEC3_EPS) {
            return {0, 0, 0};
        } else {
            return {content / l};
        }
    }

    inline bool is_zero() const {
        const auto squared = (content * content) < VEC3_EPS;
        return squared[0] * squared[1] * squared[2];
    }

    inline double dot(const Vec3d &other) const {
        const auto res = (content * other.content);
        return res[0] + res[1] + res[2];
    }

    inline Vec3d cross(const Vec3d &other) const {
        const content3 left_first = {content[1], content[2], content[0]};
        const content3 right_first = {other.content[2], other.content[0], other.content[1]};
        const content3 left_second = {content[2], content[0], content[1]};
        const content3 right_second = {other.content[1], other.content[2], other.content[0]};
        const auto res = left_first * right_first - right_second * left_second;
        return {res};
    }

    [[maybe_unused]] Vec3d orient(const Vec3d &axis);

    Vec3d &abs();

    double operator[](const int i) const {
        return content[i];
    }

    inline void set(uint8_t ind, const double &value) {
        content[ind] = value;
    }

    inline static Vec3d random() {
        return {vec3d_randdouble(), vec3d_randdouble(), vec3d_randdouble()};
    }

    inline static Vec3d random(double mn, double mx) {
        return {vec3d_randdouble(mn, mx), vec3d_randdouble(mn, mx), vec3d_randdouble(mn, mx)};
    }

    inline static Vec3d random_in_unit_sphere() {
        Vec3d rv = Vec3d::random(-1, 1);
        for(int i = 0; i < VEC3_MAX_RANDOM_TRIES_CNT && rv.len_squared() > 1; ++i, rv = Vec3d::random(-1, 1));
        return rv;
    }

    inline static Vec3d random_unit() {
        return random_in_unit_sphere().normal();
    }

    inline static double sign(const double x) {
        return fabs(x) < VEC3_EPS ? 0 : x > 0 ? 1 : -1;
    }
};

extern const Vec3d VEC3D_ZERO;
extern const Vec3d VEC3D_ONE;

inline Vec3d operator+(const Vec3d &first) {
    return first;
}

inline Vec3d operator-(const Vec3d &first) {
    return {first.content * -1};
}

inline Vec3d operator+(const Vec3d &first, const Vec3d &second) {
    return {first.content + second.content};
}

inline Vec3d operator-(const Vec3d &first, const Vec3d &second) {
    return {first.content - second.content};
}

inline Vec3d operator*(const Vec3d &first, const Vec3d &second) {
    return {first.content * second.content};
}

inline Vec3d operator/(const Vec3d &first, const Vec3d &second) {
    return {first.content / second.content};
}

inline Vec3d operator*(const Vec3d &first, const double k) {
    return {first.content * k};
}

inline Vec3d operator*(const double k, const Vec3d &first) {
    return {first.content * k};
}

inline Vec3d operator/(const Vec3d &first, const double k) {
    return {first.content / k};
}

inline Vec3d &operator+=(Vec3d &first, const Vec3d &second) {
    first.content += second.content;
    return first;
}

inline Vec3d &operator*=(Vec3d &first, const Vec3d &second) {
    first.content *= second.content;
    return first;
}

inline Vec3d &operator*=(Vec3d &first, const double k) {
    first.content *= k;
    return first;
}

inline Vec3d &operator/=(Vec3d &first, const Vec3d &second) {
    first.content /= second.content;
    return first;
}

inline Vec3d &operator/=(Vec3d &first, const double k) {
    first.content /= k;
    return first;
}

inline bool operator==(const Vec3d &first, const Vec3d &second) {
    const auto res = first.content - second.content;
    return (res[0]) < VEC3_EPS && (res[1]) < VEC3_EPS && (res[2]) < VEC3_EPS;
}

inline bool operator<(const Vec3d &first, const Vec3d &second) {
    const auto res = first.content < second.content;
    return res[0] * res[1] * res[2];
}

inline Vec3d sqrt(const Vec3d &first) {
    return {sqrt(first.content[0]),
            sqrt(first.content[1]),
            sqrt(first.content[2])};
}

inline Vec3d pow(const Vec3d &first, const double power) {
    return {pow(first.content[0], power),
            pow(first.content[1], power),
            pow(first.content[2], power)};
}

inline Vec3d oriented(Vec3d axis, Vec3d other) {
    if(axis.dot(other) < 0) {
        other *= (-1.0);
    }
    return other;
}

Vec3d rotx(const Vec3d vec, double ang);

Vec3d roty(const Vec3d vec, double ang);

Vec3d rotz(const Vec3d vec, double ang);

Vec3d rotate(const Vec3d vec, double dx, double dy, double dz);

Vec3d rotate(const Vec3d vec, const Vec3d &rotation);

inline Vec3d reflect(const Vec3d vec, Vec3d normal) {
    normal = normal.normal();
    return vec - 2 * vec.dot(normal) * normal;
}

Vec3d refract(const Vec3d vec, const Vec3d &normal, const double eta_from_over_eta_to);

double vec3d_deg_to_rad(const double deg);

double vec3d_rad_to_deg(const double rad);

#endif // GENERAL_VEC3D
