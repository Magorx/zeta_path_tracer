#ifndef CAMERA
#define CAMERA

#include "utils/vec3d.h"
#include "utils/ray.h"
#include "utils/fast_random.hpp"

struct Camera {
    Vec3d orig;
    Vec3d dir;
    double dist;
    double w;
    double h;
    int res_w;
    int res_h;
    Vec3d ort_w;
    Vec3d ort_h;
    Vec3d left_upper;

    double z_ang;
    double y_ang;

    bool using_angles;

    Camera();
    Camera(const Vec3d origin, Vec3d direction, double distance, double width, double height, double res_coef);
    Camera(const Vec3d origin, double z_ang_, double y_ang_, double distance, double width, double height, double res_coef);

    Ray get_ray(double x, double y) const;
    Ray get_sample_ray(double x, double y) const;

    void update();
};

#endif // CAMERA