#ifndef FRAME_H
#define FRAME_H

template <typename ColorT, typename NormalT, typename DepthT>
struct FramePixel {
    ColorT  color;
    NormalT normal;
    DepthT  depth;

    FramePixel(const ColorT &color_ = ColorT{0.0, 0.0, 0.0}, const NormalT &normal_ = NormalT{0.0, 0.0, 0.0}, const DepthT &depth_ = 0.0):
    color(color_),
    normal(normal_),
    depth(depth_)
    {}
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

public:
    Frame() :
    final_image(nullptr),
    data_color(nullptr),
    data_normal(nullptr),
    data_depth(nullptr),
    size_x(0),
    size_y(0),
    pixel_cnt(0)
    {}

    Frame(size_t size_x_, size_t size_y_) :
    final_image(new ColorT[size_x_ * size_y_]),
    data_color(new ColorT[size_x_ * size_y_]),
    data_normal(new NormalT[size_x_ * size_y_]),
    data_depth(new DepthT[size_x_ * size_y_]),
    size_x(size_x_),
    size_y(size_y_),
    pixel_cnt(size_x_ * size_y_)
    {}

    Frame(const ColorT *colors, const NormalT *normals, const DepthT *depths, size_t size_x_, size_t size_y_) :
    final_image(new ColorT[size_x_ * size_y_]),
    data_color(new ColorT[size_x_ * size_y_]),
    data_normal(new NormalT[size_x_ * size_y_]),
    data_depth(new DepthT[size_x_ * size_y_]),
    size_x(size_x_),
    size_y(size_y_),
    pixel_cnt(size_x_ * size_y_)
    {
        memcpy(data_color, colors, pixel_cnt);
        memcpy(data_normal, normals, pixel_cnt);
        memcpy(data_depth, depths, pixel_cnt);
    }

    FramePixel<ColorT, NormalT, DepthT> operator[](size_t i) {
        return {data_color[i], data_normal[i], data_depth[i]};
    }

    void normalize_depth_map() {
        double dmin =  data_depth[0];
        double dmax = -data_depth[0];
        for (int i = 0; i < pixel_cnt; ++i) {
            if (data_depth[i] == -1) {
                data_depth[i] = data_depth[i - 1];
            }
            dmin = std::min(dmin, data_depth[i]);
            dmax = std::max(dmax, data_depth[i]);
        }
        dmax -= dmin;
    
        for (int i = 0; i < pixel_cnt; ++i) {
            data_depth[i] = (data_depth[i] - dmin) / dmax;
        }
    }

    void colors_to_final_image() {
        for (int i = 0; i < pixel_cnt; ++i) {
            final_image[i] = data_color[i];
        }
    }

    void normals_to_final_image() {
        for (int i = 0; i < pixel_cnt; ++i) {
            final_image[i] = data_normal[i];
        }
    }

    void depths_to_final_image() {
        for (int i = 0; i < pixel_cnt; ++i) {
            final_image[i] = Color(1, 1, 1) * data_depth[i];
        }
    }
};

#endif // FRAME_H