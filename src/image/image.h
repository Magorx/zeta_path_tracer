#ifndef IMAGE_H
#define IMAGE_H

template <typename T>
struct Image {
    T *data;
    size_t x_size;
    size_t y_size;

    Image(size_t x_size_, size_t y_size_) :
    data(new T[x_size_ * y_size_]),
    x_size(x_size_),
    y_size(y_size_)
    {}

    Image(T *data_, size_t x_size_, size_t y_size_) :
    data(data_),
    x_size(x_size_),
    y_size(y_size_)
    {}

    inline bool same_size(const Image &other) const {
        return x_size == other.x_size && y_size == other.y_size;
    }

    T &operator[](size_t i) {
        return *(data + i * y_size);
    }
    
    const T &operator[](size_t i) const {
        return *(data + i * y_size);
    }

    T &operator()(size_t i, size_t j) {
        return *(data + i * y_size + j);
    }
    
    const T &operator()(size_t i, size_t j) const {
        return *(data + i * y_size + j);
    }

    inline size_t width() const {
        return x_size;
    }

    inline size_t height() const {
        return y_size;
    }
};

#endif // IMAGE_H