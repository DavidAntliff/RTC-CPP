#ifndef RTC_LIB_COLOR_H
#define RTC_LIB_COLOR_H

#include "tuples.h"

namespace rtc {

template <typename Base=Tuple<fp_t>>
struct Color : Base {
    using value_t = Base::value_t;

    Color() = default;
    Color(value_t red, value_t green, value_t blue) :
        Base(red, green, blue, 0) {}

    // Rule of 5
    virtual ~Color() = default;
    Color(Color &&) = default;
    Color& operator=(Color &&) = default;
    Color(const Color&) = default;
    Color& operator=(const Color&) = default;

    value_t red() const { return Base::x_; }
    value_t green() const { return Base::y_; }
    value_t blue() const { return Base::z_; }

    Color & operator*=(const Color & rhs) {
        Base::x_ *= rhs.red();
        Base::y_ *= rhs.green();
        Base::z_ *= rhs.blue();
        return *this;
    }

    // Convert a Tuple into a Color
    Color(Base const & t) : Base{t} {}

};

template <typename T>
inline bool almost_equal(const Color<T> & lhs, const Color<T> & rhs) {
    return almost_equal(lhs.red(), rhs.red())
           && almost_equal(lhs.green(), rhs.green())
           && almost_equal(lhs.blue(), rhs.blue());
}

// Hadamard or Shur Product
template <typename T>
inline Color<T> operator*(Color<T> lhs, const Color<T> & rhs)
{
    lhs *= rhs;
    return lhs;
}

// This is broken in the language, as template types can't be deduced:
//template<typename T>
//const auto hadamard = operator*<T>;

// Instead, use a wrapper function:
template <typename T>
T hadamard(const Color<T> & lhs, const Color<T> & rhs) {
    return lhs * rhs;
}

template <typename T=Tuple<>>
inline auto color(
        typename Color<T>::value_t r,
        typename Color<T>::value_t g,
        typename Color<T>::value_t b) {
    return Color<T> {r, g, b};
}

} // namespace rtc

#endif // RTC_LIB_COLOR_H
