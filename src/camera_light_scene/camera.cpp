#include "camera.h"

Camera::Camera () {}

Camera::Camera(const Vec3d origin, Vec3d direction, double distance, double width, double height, double res_coef) {
    orig = origin;
    dir = direction;
    dist = distance;
    w = width;
    h = height;
    res_w = width  * res_coef;
    res_h = height * res_coef;

    dir.normalize();

    ort_h = {0, 0, 1}; // this does not work for not horizontal looking camera
    ort_w = -dir.cross(ort_h).normal();
    left_upper = orig + dir * dist + ort_w * width * 0.5 + ort_h * height * 0.5;

    ort_w *= -1;
    ort_h *= -1;

    Vec3d a = left_upper;
    a += ort_w * res_w * w / res_w;
    a += ort_h * res_h * h / res_h;
}

Ray Camera::get_ray(double x, double y) const {
    Vec3d dw = ort_w;
    Vec3d dh = ort_h;

    dw *= (x * w / res_w);
    dh *= (y * h / res_h);

    Vec3d ray_dir = left_upper;

    ray_dir += dw;
    ray_dir += dh;
    ray_dir -= orig;

    return {orig, ray_dir};
}

Ray Camera::get_sample_ray(double x, double y) const {
    unsigned int random_values[4];
    Brans::rand_sse(random_values);
    double rx = (double) random_values[0] / (double) UINT32_MAX + x;
    double ry = (double) random_values[1] / (double) UINT32_MAX + y;
    return get_ray(rx, ry);
}

void Camera::update() {
    // ort_w = Vec3d(-dir.y, dir.x, dir.z).normal();
    // ort_h = ort_w.cross(dir).normal();
    ort_h = {0, 0, 1}; // this does not work for not horizontal looking camera
    ort_w = -dir.cross(ort_h).normal();
    left_upper = orig + dir * dist + ort_w * w * 0.5 + ort_h * h * 0.5;

    ort_w *= -1;
    ort_h *= -1;
}
