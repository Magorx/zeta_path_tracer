#ifndef DENOISER_H
#define DENOISER_H

#include "color/color.h"
#include "image/image.h"
#include "image/frame.h"

#include <algorithm>

void denoise(Frame<Color, Vec3d, double> &frame, const int sample_rad=1);

void intelligence_denoise(Frame<Color, Vec3d, double> &frame, const int sample_rad=1);

#endif // DENOISER_H