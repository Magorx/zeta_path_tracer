#ifndef CAMERA
#define CAMERA

#include "vec3d.h"
#include "ray.h"

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

    Camera();
    Camera(const Vec3d origin, Vec3d direction, double distance, double width, double height, double res_coef);
    void update();

    Ray get_ray(double x, double y) const;
    Ray get_sample_ray(double x, double y) const;
};

#endif // CAMERA