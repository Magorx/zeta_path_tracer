#ifndef FRAME_H
#define FRAME_H

#include "image/denoiser.h"

enum class FramePostproc {
    copy,
    normal,
    depth,
    denoise
};

template <typename ColorT, typename NormalT, typename DepthT>
struct Frame {
    ColorT *final_image;

    ColorT  *data_color;
    NormalT *data_normal;
    DepthT  *data_depth;
    
    size_t size_x;
    size_t size_y;

    size_t pixel_cnt;

    void (Frame::*post_processing)(const int sample_rad);

public:
    Frame() :
    final_image(nullptr),
    data_color(nullptr),
    data_normal(nullptr),
    data_depth(nullptr),
    size_x(0),
    size_y(0),
    pixel_cnt(0),
    post_processing(&Frame::colors_to_final_image)
    {}

    Frame(size_t size_x_, size_t size_y_) :
    final_image(new ColorT[size_x_ * size_y_]),
    data_color(new ColorT[size_x_ * size_y_]),
    data_normal(new NormalT[size_x_ * size_y_]),
    data_depth(new DepthT[size_x_ * size_y_]),
    size_x(size_x_),
    size_y(size_y_),
    pixel_cnt(size_x_ * size_y_),
    post_processing(&Frame::colors_to_final_image)
    {}

    Frame(const ColorT *colors, const NormalT *normals, const DepthT *depths, size_t size_x_, size_t size_y_) :
    final_image(new ColorT[size_x_ * size_y_]),
    data_color(new ColorT[size_x_ * size_y_]),
    data_normal(new NormalT[size_x_ * size_y_]),
    data_depth(new DepthT[size_x_ * size_y_]),
    size_x(size_x_),
    size_y(size_y_),
    pixel_cnt(size_x_ * size_y_),
    post_processing(&Frame::colors_to_final_image)
    {
        memcpy(data_color, colors, pixel_cnt);
        memcpy(data_normal, normals, pixel_cnt);
        memcpy(data_depth, depths, pixel_cnt);
    }

    void normalize_depth_map() {
        double dmin =  data_depth[0];
        double dmax = -data_depth[0];
        for (size_t i = 0; i < pixel_cnt; ++i) {
            if (data_depth[i] == -1) {
                data_depth[i] = data_depth[i - 1];
            }
            dmin = std::min(dmin, data_depth[i]);
            dmax = std::max(dmax, data_depth[i]);
        }
        dmax -= dmin;
    
        for (size_t i = 0; i < pixel_cnt; ++i) {
            data_depth[i] = (data_depth[i] - dmin) / dmax;
        }
    }

    void set_post_processing(FramePostproc proc) {
        switch (proc)
        {
        case FramePostproc::copy:
            post_processing = &Frame::colors_to_final_image;
            break;
        
        case FramePostproc::normal:
            post_processing = &Frame::normals_to_final_image;
            break;
        
        case FramePostproc::depth:
            post_processing = &Frame::depths_to_final_image;
            break;
        
        case FramePostproc::denoise:
            post_processing = &Frame::denoise_intelligence_to_final_image;
            break;
        
        default:
            post_processing = &Frame::colors_to_final_image;
        }
    }

    void postproc(int sample_rad=0) {
        (this->*post_processing)(sample_rad);
    }

private:
    void colors_to_final_image(const int) {
        for (size_t i = 0; i < pixel_cnt; ++i) {
            final_image[i] = data_color[i];
        }
    }

    void normals_to_final_image(const int) {
        for (size_t i = 0; i < pixel_cnt; ++i) {
            final_image[i] = data_normal[i];
        }
    }

    void depths_to_final_image(const int) {
        for (size_t i = 0; i < pixel_cnt; ++i) {
            final_image[i] = Color(255, 255, 255) * (1 - data_depth[i]);
        }
    }

    void denoise_intelligence_to_final_image(const int sample_rad=1) {
        normalize_depth_map();
        intelligence_denoise(*this, sample_rad);
    }

};

#endif // FRAME_H