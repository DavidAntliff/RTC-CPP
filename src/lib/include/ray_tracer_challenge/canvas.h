#ifndef RTC_LIB_CANVAS_H
#define RTC_LIB_CANVAS_H

#include <vector>
#include <optional>
#include <string>

#include <boost/format.hpp>

#include "color.h"

namespace rtc {

namespace detail {

template <typename T>
void add_value(std::string & row, T value) {
    const auto v = std::min(std::max(value, 0.0), 1.0);
    const unsigned int ivalue = std::rint(v * 255);
    if (!row.empty()) {
        row += ' ';
    }
    row += std::to_string(ivalue);
}

void split_line_by(std::vector<std::string> & lines,
                   std::string_view line, int limit) {
    if (line.length() > 70) {
        const auto idx = line.rfind(' ', 70 - 1);
        lines.emplace_back(line.substr(0, idx));
        line = line.substr(idx + 1);
        split_line_by(lines, line, limit);
    } else {
        lines.emplace_back(line);
    }
}

} // namespace detail

template <typename PixelType>
class Canvas {
public:
    using pixel_t = PixelType;

    Canvas() = default;
    Canvas(unsigned int width, unsigned int height) :
        width_(width), height_(height),
        pixels_(width_ * height_) {}

    unsigned int width() const { return width_; }
    unsigned int height() const { return height_; }

    std::optional<PixelType> pixel_at(unsigned int x, unsigned int y) const {
        auto index = index_of_(x, y);
        if (index) {
            return pixels_[*index];
        } else {
            return std::nullopt;
        }
    }

    void write_pixel(unsigned int x, unsigned int y, PixelType p) {
        auto index = index_of_(x, y);
        if (index) {
            pixels_[*index] = p;
        }
    }

private:
    std::optional<unsigned int> index_of_(unsigned int x, unsigned int y) const {
        if (x < width_ && y < height_) {
            return x + y * width_;
        }
        return std::nullopt;
    }

private:
    unsigned int width_ {};
    unsigned int height_ {};

    // Store pixels row by row, starting with
    // the top left (x = 0, y = 0)
    std::vector<PixelType> pixels_ {};
};

template <typename PixelType=Color<>>
auto canvas(unsigned int width, unsigned int height) {
    return Canvas<PixelType> {width, height};
}

template <typename PixelType=Color<>>
auto pixel_at(Canvas<PixelType> const & canvas, unsigned int x, unsigned int y) {
    return canvas.pixel_at(x, y);
}

template <typename Canvas>
auto write_pixel(Canvas & canvas, unsigned int x, unsigned int y, typename Canvas::pixel_t const & color) {
    canvas.write_pixel(x, y, color);
}

template <typename Canvas>
auto ppm_from_canvas(Canvas const & canvas) {
    const auto header = (boost::format("P3\n%1% %2%\n255\n") % canvas.width() % canvas.height()).str();

    std::string data;

    for (auto y = 0U; y < canvas.height(); ++y) {
        std::string row;
        for (auto x = 0U; x < canvas.width(); ++x) {
            const auto p = canvas.pixel_at(x, y);

            detail::add_value(row, p->red());
            detail::add_value(row, p->green());
            detail::add_value(row, p->blue());
        }

        std::vector<std::string> lines;
        detail::split_line_by(lines, row, 70);

        for (auto & i : lines) {
            data += i + '\n';
        }
    }

    return header + data;
}

} // namespace rtc

#endif // RTC_LIB_CANVAS_H
