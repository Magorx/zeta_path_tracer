#ifndef DENOISER_H
#define DENOISER_H

#include "color/color.h"
#include "image/image.h"
#include "image/frame.h"

void denoise(const Image<Color> &input, Image<Color> &output, const int sample_rad=1);

void intelligence_denoise(Frame<Color, Vec3d, double> &frame, const int sample_rad=1);

#endif // DENOISER_H