#ifndef RTC_LIB_CANVAS_H
#define RTC_LIB_CANVAS_H

#include <vector>
#include <optional>
#include <string>

#include <boost/format.hpp>

#include "color.h"

namespace rtc {

template <typename PixelType>
class Canvas {
public:
    using pixel_t = PixelType;

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

    void write_pixel(int x, int y, PixelType p) {
        auto index = index_of_(x, y);
        if (index) {
            pixels_[*index] = p;
        }
    }

private:
    std::optional<int> index_of_(int x, int y) const {
        if (x >= 0 && x < width_ &&
            y >= 0 && y < height_) {
            return x + y * width_;
        }
        return {};
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

template <typename Canvas>
auto write_pixel(Canvas & canvas, int x, int y, const typename Canvas::pixel_t & color) {
    canvas.write_pixel(x, y, color);
}

template <typename T>
void add_value(std::string & row, T value) {
    value = std::min(std::max(value, 0.0), 1.0);
    const int ivalue = std::round(value * 255);
    if (!row.empty()) {
        row += ' ';
    }
    row += std::to_string(ivalue);
}

void split_line_by(std::vector<std::string> & lines,
                   std::string_view line, int limit) {
    if (line.length() > 70) {
        const auto idx = line.rfind(' ', 70 - 1);
        lines.push_back(std::string(line.substr(0, idx)));
        line = line.substr(idx + 1);
        split_line_by(lines, line, limit);
    } else {
        lines.push_back(std::string(line));
    }
}

template <typename Canvas>
auto ppm_from_canvas(Canvas & canvas) {
    const auto header = (boost::format("P3\n%1% %2%\n255\n") % canvas.width() % canvas.height()).str();

    std::string data;

    for (auto y = 0; y < canvas.height(); ++y) {
        std::string row;
        for (auto x = 0; x < canvas.width(); ++x) {
            const auto p = canvas.pixel_at(x, y);

            add_value(row, p->red());
            add_value(row, p->green());
            add_value(row, p->blue());
        }

        std::vector<std::string> lines;
        split_line_by(lines, row, 70);

        for (auto & i : lines) {
            data += i + '\n';
        }
    }

    return header + data;
}

} // namespace rtc

#endif // RTC_LIB_CANVAS_H
