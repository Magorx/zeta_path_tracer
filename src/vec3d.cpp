#include "vec3d.h"

const Vec3d VEC3D_ZERO(0, 0, 0);
const Vec3d VEC3D_ONE (1, 1, 1);

long vec3d_randlong() {
    long ret = rand();
    ret |= rand() << sizeof(int);
    return ret > 0 ? ret : -ret;
}

double vec3d_randdouble(double dmin, double dmax) {
    double x = (double)rand() / RAND_MAX;
    return dmin + x * (dmax - dmin);
}

Vec3d::Vec3d():
x(0.0),
y(0.0),
z(0.0)
{}

Vec3d::Vec3d(double x_, double y_, double z_):
x(x_),
y(y_),
z(z_)
{}

double Vec3d::len() const {
    return sqrt(x * x + y * y + z * z);
}

double Vec3d::len_squared() const {
    return x * x + y * y + z * z;
}

Vec3d Vec3d::normal() const {
    double l = len();
    if (l < VEC3_EPS) {
        return {0, 0, 0};
    } else {
        return {x / l, y / l, z / l};
    }
}

bool Vec3d::is_zero() const {
    return fabs(x) < VEC3_EPS && fabs(y) < VEC3_EPS && fabs(z) < VEC3_EPS;
}

double Vec3d::dot(const Vec3d &other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec3d Vec3d::cross(const Vec3d &other) const {
    double res_x = y * other.z - z * other.y;
    double res_y = z * other.x - x * other.z;
    double res_z = x * other.y - y * other.x;
    return {res_x, res_y, res_z};
}

double Vec3d::operator[](const int i) const {
    switch(i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            return 0;
    }
}

Vec3d operator+(const Vec3d &first) {
    return first;
}

Vec3d operator-(const Vec3d &first) {
    return {-first.x, -first.y, -first.z};
}

Vec3d operator+(const Vec3d &first, const Vec3d &second) {
    return {first.x + second.x, first.y + second.y, first.z + second.z};
}

Vec3d operator+=(Vec3d &first, const Vec3d &second) {
    first.x += second.x;
    first.y += second.y;
    first.z += second.z;
    return first;
}

Vec3d operator-(const Vec3d &first, const Vec3d &second) {
    return {first.x - second.x, first.y - second.y, first.z - second.z};
}

Vec3d operator*(const Vec3d &first, const Vec3d &second) {
    return {first.x * second.x, first.y * second.y, first.z * second.z};
}

Vec3d operator*=(Vec3d &first, const Vec3d &second) {
    first.x *= second.x;
    first.y *= second.y;
    first.z *= second.z;
    return first;
}

Vec3d operator*(const Vec3d &first, const double k) {
    return {first.x * k, first.y * k, first.z * k};
}

Vec3d operator*(const double k, const Vec3d &first) {
    return {first.x * k, first.y * k, first.z * k};
}

Vec3d operator*=(Vec3d &first, const double k) {
    first.x *= k;
    first.y *= k;
    first.z *= k;
    return first;
}

Vec3d operator/(const Vec3d &first, const Vec3d &second) {
    return {first.x / second.x, first.y / second.y, first.z / second.z};
}

Vec3d operator/(const Vec3d &first, const double k) {
    return {first.x / k, first.y / k, first.z / k};
}

bool operator==(const Vec3d &first, const Vec3d &second) {
    return (first.x - second.x) < VEC3_EPS  && (first.y - second.y) < VEC3_EPS && (first.z -second.z) < VEC3_EPS;
}

Vec3d sqrt(const Vec3d &first) {
    return {sqrt(first.x), sqrt(first.y), sqrt(first.z)};
}

Vec3d pow (const Vec3d &first, const double power) {
    return {pow(first.x, power), pow(first.y, power), pow(first.z, power)};
}

bool operator<(const Vec3d &first, const Vec3d &second) {
    return first.x < second.x  && first.y < second.y && first.z < second.z;
}

std::ostream& operator<<(std::ostream& output_stream, const Vec3d &vector) {
    output_stream << vector.x << ' ' << vector.y << ' ' << vector.z;
    return output_stream;
}

std::istream& operator>>(std::istream& input_stream, Vec3d &vector) {
    char coma, bracket;
    input_stream >> bracket >> vector.x >> coma >> vector.y >> coma >> vector.z >> bracket;

    return input_stream;
}

Vec3d oriented(Vec3d axis, Vec3d other) {
    if (axis.dot(other) < 0) {
        other = other * (-1.0);
    }
    return other;
}

Vec3d Vec3d::orient(const Vec3d &axis) {
    if (axis.dot(*this) < 0) {
        *this *= -1.0;
    }
    return *this;
}

Vec3d rotx(const Vec3d vec, double ang) {
    double x = vec.x;
    double y = vec.y * cos(ang) - vec.z * sin(ang);
    double z = vec.z * cos(ang) + vec.y * sin(ang);
    return {x, y, z};
}

Vec3d roty(const Vec3d vec, double ang) {
    double x = vec.x * cos(ang) + vec.z * sin(ang);
    double y = vec.y;
    double z = vec.z * cos(ang) - vec.x * sin(ang);
    return {x, y, z};
}

Vec3d rotz(const Vec3d vec, double ang) {
    double x = vec.x * cos(ang) - vec.y * sin(ang);
    double y = vec.y * cos(ang) - vec.x * sin(ang);
    double z = vec.z;
    return {x, y, z};
}

Vec3d rotate(const Vec3d vec, double dx, double dy, double dz) {
    return rotz(roty(rotx(vec, dx), dy), dz);
}

Vec3d rotate(const Vec3d vec, const Vec3d rotation) {
    return rotz(roty(rotx(vec, rotation.x), rotation.y), rotation.z);
}

Vec3d reflect(const Vec3d vec, Vec3d normal) {
    normal = normal.normal();
    return vec - 2 * vec.dot(normal) * normal;
}

Vec3d refract(const Vec3d vec, const Vec3d &normal, const double eta_from_over_eta_to) {
    Vec3d uv = vec.normal();
    Vec3d n = normal.normal();

    auto cos_theta = fmin(-uv.dot(n), 1.0);
    Vec3d r_out_perp =  eta_from_over_eta_to * (uv + cos_theta * n);
    Vec3d r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.len_squared())) * n;
    return r_out_perp + r_out_parallel;
}
