#include "marble.h"
#include "solid_color.h"


#include <vector>


t_Marble::t_Marble(Texture *first_, Texture *second_, const Vec3d &scale_)
    : Texture()
    , first(first_ ? first_ : new t_SolidColor({255, 255, 255}))
    , second(second_ ? second_ : new t_SolidColor({0, 0, 0}))
    , noise(scale_)
{}

t_Marble::t_Marble(const Color &first_, const Color &second_, const Vec3d &scale_)
    : t_Marble(
        new t_SolidColor(first_),
        new t_SolidColor(second_),
        scale_
    ) {
}


Color t_Marble::value(double sx, double sy, const Vec3d &point) const {
    double noise1 = (1 + sin(point.z() * noise.get_scale().z() + 1 / noise.get_scale().z() * noise.turbulense(point * noise.get_scale()))) * 0.5;
    double noise2 = (1 + sin(point.y() * noise.get_scale().y() + 2 / noise.get_scale().y() * noise.turbulense(point * noise.get_scale()))) * 0.5;

    std::vector<double> noises {
        noise1,
        noise2
    };
    double noise = 1;
    for (auto x : noises) {
        noise = std::min(noise, x);
    }

    if (noise > 0.5) {
        noise = 1;
    } else {
        noise = 0;
    }
    
    return first->value(sx, sy, point) * (1 - noise) + second->value(sx, sy, point) * noise;
}
