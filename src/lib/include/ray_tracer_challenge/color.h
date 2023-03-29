#ifndef RTC_LIB_COLOR_H
#define RTC_LIB_COLOR_H

#include "tuples.h"

namespace rtc {

template <typename T=fp_t>
struct Color : Tuple<T> {
    using Base = Tuple<T>;
    using value_t = T;

    Color() = default;
    Color(value_t red, value_t green, value_t blue) :
        Base(red, green, blue, 0) {}

    value_t red() const { return Base::x_; }
    value_t green() const { return Base::y_; }
    value_t blue() const { return Base::z_; }

    Color & operator*=(Color const & rhs) {
        Base::x_ *= rhs.red();
        Base::y_ *= rhs.green();
        Base::z_ *= rhs.blue();
        return *this;
    }

    // Convert a Tuple into a Color
    Color(Base const & t) : Base{t} {}

};

template <typename T>
inline bool almost_equal(Color<T> const & lhs, Color<T> const & rhs) {
    return almost_equal(lhs.red(), rhs.red())
           && almost_equal(lhs.green(), rhs.green())
           && almost_equal(lhs.blue(), rhs.blue());
}

// Hadamard or Shur Product
template <typename T>
inline Color<T> operator*(Color<T> lhs, Color<T> const & rhs)
{
    lhs *= rhs;
    return lhs;
}

// This is broken in the language, as template types can't be deduced:
//template<typename T>
//const auto hadamard = operator*<T>;

// Instead, use a wrapper function:
template <typename T>
Color<T> hadamard(Color<T> const & lhs, Color<T> const & rhs) {
    return lhs * rhs;
}

template <typename T>
inline auto color(T r, T g, T b) {
    return Color<T> {r, g, b};
}

template <>
inline auto color<int>(int r, int g, int b) {
    return Color<fp_t> {r / 255.0, g / 255.0, b / 255.0};
}

// Blending function (linear interpolation)
template <typename T>
inline auto color(T t, Color<T> const & a, Color<T> const & b) {
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
