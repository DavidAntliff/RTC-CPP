#ifndef RTC_LIB_CANVAS_H
#define RTC_LIB_CANVAS_H

#include <vector>
#include <optional>

#include "color.h"

namespace rtc {

template <typename PixelType>
class Canvas {
public:
    Canvas(int width, int height) :
        width_(width), height_(height),
        pixels_(width_ * height_) {}

//    Canvas (const Canvas &) = delete;
//    Canvas & operator= (const Canvas &) = delete;
//
//    Canvas(Canvas && other) :
//        width_(std::move(other.width_)),
//        height_(std::move(other.height_)),
//        pixels_(std::move(other.pixels_)) {}

    int width() const { return width_; }
    int height() const { return height_; }

    std::optional<PixelType> pixel_at(int x, int y) const {
        auto index = index_of_(x, y);
        if (index) {
            return pixels_[*index];
        } else {
            return {};
        }
    }

private:
    std::optional<int> index_of_(int x, int y) const {
        return x + y * width_;
    }

private:
    int width_;
    int height_;

    // Store pixels row by row, starting with
    // the top left (x = 0, y = 0)
    std::vector<PixelType> pixels_;
};

template <typename PixelType=Color<>>
auto canvas(int width, int height) {
    return Canvas<PixelType>(width, height);
}

template <typename PixelType=Color<>>
auto pixel_at(const Canvas<PixelType> & canvas, int x, int y) {
    return canvas.pixel_at(x, y);
}

} // namespace rtc

#endif // RTC_LIB_CANVAS_H
