#include "rgba.h"

RGBA::RGBA() :
i(0)
{}

RGBA::RGBA(const int x) :
i(x)
{}

RGBA::RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_):
r(r_),
g(g_),
b(b_),
a(a_)
{}

RGBA RGBA::operator+(const RGBA &other) const {
    return {(unsigned char) (r + other.r),
            (unsigned char) (g + other.g),
            (unsigned char) (b + other.b),
            (unsigned char) (a + other.a)};
}

RGBA RGBA::operator/(const float &coef) const {
    return {(unsigned char) (r / coef),
            (unsigned char) (g / coef),
            (unsigned char) (b / coef),
            (unsigned char) (a / coef)};
}

RGBA RGBA::operator*(const float &coef) const {
    return {(unsigned char) (r * coef),
            (unsigned char) (g * coef),
            (unsigned char) (b * coef),
            (unsigned char) (a * coef)};
}


