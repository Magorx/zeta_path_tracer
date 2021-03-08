#ifndef GENERAL_VEC3D
#define GENERAL_VEC3D

#include <cmath>
#include <iostream>

const double VEC3_EPS = 1e-6;
const double VEC3_MAX_RANDOM_TRIES_CNT = 50;
const double VEC3D_INF = 1e9;

//=============================================================================
//<KCTF> Random for vector ====================================================

long vec3d_randlong();

double vec3d_randdouble(double dmin = 0.0, double dmax = 1.0);

//=============================================================================
//<KCTF> Vec2_header ==========================================================

struct Vec3d {
    double x;
    double y;
    double z;

    Vec3d();
    Vec3d(double x_, double y_, double z_);

    double  len()         const;
    double  len_squared() const;
    Vec3d   normal()      const;

    bool is_zero() const;

    double  dot(const Vec3d &other) const;
    Vec3d cross(const Vec3d &other) const;

    Vec3d orient(const Vec3d &axis);

    double operator[](const int i) const;

    inline static Vec3d random() {
        return {vec3d_randdouble(), vec3d_randdouble(), vec3d_randdouble()};
    }

    inline static Vec3d random(double mn, double mx) {
        return {vec3d_randdouble(mn, mx), vec3d_randdouble(mn, mx), vec3d_randdouble(mn, mx)};
    }

    inline static Vec3d random_in_unit_sphere() {
        Vec3d rv = Vec3d::random(-1, 1);
        for (int i = 0; i < VEC3_MAX_RANDOM_TRIES_CNT && rv.len_squared() > 1; ++i, rv = Vec3d::random(-1, 1));
        return rv;
    }

    inline static Vec3d random_unit() {
        return random_in_unit_sphere().normal();
    }
};

extern const Vec3d VEC3D_ZERO;

Vec3d operator+  (const Vec3d &first);
Vec3d operator-  (const Vec3d &first);

Vec3d operator+  (const Vec3d &first, const Vec3d &second);
Vec3d operator-  (const Vec3d &first, const Vec3d &second);
Vec3d operator*  (const Vec3d &first, const Vec3d &second);
Vec3d operator/  (const Vec3d &first, const Vec3d &second);

Vec3d operator*  (const Vec3d &first, const double k);
Vec3d operator*  (const double k, const Vec3d &first);
Vec3d operator/  (const Vec3d &first, const double k);

Vec3d operator+= (Vec3d &first, const Vec3d &second);
Vec3d operator*= (Vec3d &first, const Vec3d &second);
Vec3d operator*= (Vec3d &first, const double k);

bool operator==     (const Vec3d &first, const Vec3d &second);
bool operator<      (const Vec3d &first, const Vec3d &second);

Vec3d sqrt(const Vec3d &first);
Vec3d pow (const Vec3d &first, const double power);

std::ostream& operator<<(std::ostream& output_stream, const Vec3d &vector);
std::istream& operator>>(std::istream& input_stream, Vec3d &vector);

Vec3d oriented(Vec3d axis, Vec3d other);

Vec3d rotx(const Vec3d vec, double ang);
Vec3d roty(const Vec3d vec, double ang);
Vec3d rotz(const Vec3d vec, double ang);

Vec3d rotate(const Vec3d vec, double dx, double dy, double dz);
Vec3d rotate(const Vec3d vec, const Vec3d &rotation);

Vec3d reflect(const Vec3d vec, Vec3d normal);
Vec3d refract(const Vec3d vec, const Vec3d &normal, const double eta_from_over_eta_to);

#endif // GENERAL_VEC3D