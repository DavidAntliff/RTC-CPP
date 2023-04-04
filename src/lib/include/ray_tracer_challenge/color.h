#ifndef RTC_LIB_COLOR_H
#define RTC_LIB_COLOR_H

#include "tuples.h"

namespace rtc {

struct Color : Tuple {

    Color() = default;
    Color(fp_t red, fp_t green, fp_t blue) :
        Tuple(red, green, blue, 0) {}

    fp_t red() const { return Tuple::x_; }
    fp_t green() const { return Tuple::y_; }
    fp_t blue() const { return Tuple::z_; }

    Color & operator*=(Color const & rhs) {
        Tuple::x_ *= rhs.red();
        Tuple::y_ *= rhs.green();
        Tuple::z_ *= rhs.blue();
        return *this;
    }

    // Convert a Tuple into a Color
    Color(Tuple const & t) : Tuple{t} {}

};

inline bool almost_equal(Color const & lhs, Color const & rhs) {
    return almost_equal(lhs.red(), rhs.red())
           && almost_equal(lhs.green(), rhs.green())
           && almost_equal(lhs.blue(), rhs.blue());
}

// Resolve ambiguity
inline bool almost_equal(Tuple const & lhs, Color const & rhs) {
    return almost_equal(lhs.x(), rhs.red())
           && almost_equal(lhs.y(), rhs.green())
           && almost_equal(lhs.z(), rhs.blue());
}

// Resolve ambiguity
inline bool almost_equal(Color const & lhs, Tuple const & rhs) {
    return almost_equal(lhs.red(), rhs.x())
           && almost_equal(lhs.green(), rhs.y())
           && almost_equal(lhs.blue(), rhs.z());
}

// Hadamard or Shur Product
inline Color operator*(Color lhs, Color const & rhs)
{
    lhs *= rhs;
    return lhs;
}

// Function alias
const auto hadamard = static_cast<Color(*)(Color, Color const &)>(operator*);

template <typename T>
inline auto color(T r, T g, T b) {
    return Color {r, g, b};
}

template <>
inline auto color<int>(int r, int g, int b) {
    return Color {r / 255.0, g / 255.0, b / 255.0};
}

// Blending function (linear interpolation)
inline auto color(fp_t t, Color const & a, Color const & b) {
    auto const distance {b - a};
    auto const fraction {t - floor(t)};
    return a + distance * fraction;
}

const auto black {color(0.0, 0.0, 0.0)};
const auto red {color(1.0, 0.0, 0.0)};
const auto green {color(0.0, 1.0, 0.0)};
const auto blue {color(0.0, 0.0, 1.0)};
const auto yellow {color(1.0, 1.0, 0.0)};
const auto cyan {color(0.0, 1.0, 1.0)};
const auto magenta {color(1.0, 1.0, 0.0)};
const auto white {color(1.0, 1.0, 1.0)};

} // namespace rtc

#endif // RTC_LIB_COLOR_H
