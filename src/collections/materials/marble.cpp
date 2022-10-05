#include "marble.h"
#include "lambertian.h"

#include <vector>


m_Marble::m_Marble(Material *first_, Material *second_, const Vec3d &scale_)
    : Material()
    , first(first_ ? first_ : new m_Lambertian({255, 255, 255}))
    , second(second_ ? second_ : new m_Lambertian({0, 0, 0}))
    , noise(scale_)
{}

double m_Marble::get_noise(const Vec3d &point) const {
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

    return noise;
}

bool m_Marble::scatter(const Ray &ray, const HitRecord &hitrec, Color &attenuation, Ray &scattered) const {
    if (get_noise(hitrec.point) > 0.5) {
       return first->scatter(ray, hitrec, attenuation, scattered);
    } else {
        return second->scatter(ray, hitrec, attenuation, scattered);
    }
}

Vec3d m_Marble::emit(const double surf_x, const double surf_y, const Vec3d &point) const {
    if (get_noise(point) > 0.5) {
        return first->emit(surf_x, surf_y, point);
    } else {
        return second->emit(surf_x, surf_y, point);
    }
}

void m_Marble::affect_emitter(Vec3d &emitted_color, const double surf_x, const double surf_y, const Vec3d &point) const {
    if (get_noise(point) > 0.5) {
        first->affect_emitter(emitted_color, surf_x, surf_y, point);
    } else {
        second->affect_emitter(emitted_color, surf_x, surf_y, point);
    }
}
