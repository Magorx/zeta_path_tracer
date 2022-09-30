#pragma once


#include "utils_header.h"


// image texture class
class t_Image : public Texture {
    unsigned char *data;
    int width;
    int height;
    int channels;
public:
    t_Image() {}
    t_Image(const char *filename);
    ~t_Image();

    virtual Color value(double u, double v, const Vec3d &p) const override;
};
