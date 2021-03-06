#include "camera.h"

Camera::Camera () {}

Camera::Camera(const Vec3d origin, Vec3d direction, double distance, double width, double height, double res_coef) {
    orig = origin;
    dir = direction.normal();
    dist = distance;
    w = width;
    h = height;
    res_w = width * res_coef;
    res_h = height * res_coef;

    ort_w = Vec3d(-dir.y, dir.x, dir.z).normal();
    ort_h = ort_w.cross(dir).normal();
    left_upper = orig + direction * dist + ort_w * width * 0.5 + ort_h * height * 0.5;

    ort_w *= -1;
    ort_h *= -1;
}

Ray Camera::get_ray(double x, double y) const {
    Vec3d dw = ort_w * x * w / res_w;
    Vec3d dh = ort_h * y * h / res_h;
    return {orig, (left_upper + dw + dh).normal()};
}

Ray Camera::get_sample_ray(double x, double y) const {
    return get_ray(vec3d_randdouble(x, x + 1.0), vec3d_randdouble(y, y + 1.0));
}

void Camera::update() {
    ort_w = Vec3d(-dir.y, dir.x, dir.z).normal();
    ort_h = ort_w.cross(dir).normal();
    left_upper = orig + dir * dist + ort_w * w * 0.5 + ort_h * h * 0.5;

    ort_w *= -1;
    ort_h *= -1;
}
