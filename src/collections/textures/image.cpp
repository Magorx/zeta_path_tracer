#include "image.h"


#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include <extern/stb_image.h>


t_Image::t_Image(const char *filename) {
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (data) {
        this->width = width;
        this->height = height;
        this->channels = channels;
        this->data = data;
    } else {
        throw std::runtime_error("Failed to load image");
    }
}
t_Image::~t_Image() {
    stbi_image_free(data);
}

Color t_Image::value(double u, double v, const Vec3d &) const {
    // if we have no texture data, then return solid cyan as a debugging aid
    if (data == nullptr) {
        return Color(0, 255, 255);
    }

    // clamp input texture coordinates to [0,1] x [1,0]
    u = std::clamp(u, 0.0, 1.0);
    v = 1.0 - std::clamp(v, 0.0, 1.0);  // Flip V to image coordinates

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width)  i = width - 1;
    if (j >= height) j = height - 1;

    const auto color_scale = 1.0;
    auto pixel = data + j * width * channels + i * channels;

    return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}
