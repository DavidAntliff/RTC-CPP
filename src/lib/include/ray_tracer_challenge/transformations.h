#ifndef RTC_LIB_TRANSFORMATIONS_H
#define RTC_LIB_TRANSFORMATIONS_H

#include <cmath>

#include "matrix.h"

namespace rtc {

template <typename T>
inline auto translation(T x, T y, T z) {
    return Matrix<T, 4> {
        {T(1), T(0), T(0), x},
        {T(0), T(1), T(0), y},
        {T(0), T(0), T(1), z},
        {T(0), T(0), T(0), T(1)},
    };
}

template <typename T>
inline auto scaling(T x, T y, T z) {
    return Matrix<T, 4> {
        {   x, T(0), T(0), T(0)},
        {T(0),    y, T(0), T(0)},
        {T(0), T(0),    z, T(0)},
        {T(0), T(0), T(0), T(1)},
    };
}

template <typename T>
inline auto rotation_x(T radians) {
    const T cos_r = std::cos(radians);
    const T sin_r = std::sin(radians);
    return Matrix<T, 4> {
        {T(1),  T(0),   T(0), T(0)},
        {T(0), cos_r, -sin_r, T(0)},
        {T(0), sin_r,  cos_r, T(0)},
        {T(0),  T(0),   T(0), T(1)},
    };
}

template <typename T>
inline auto rotation_y(T radians) {
    const T cos_r = std::cos(radians);
    const T sin_r = std::sin(radians);
    return Matrix<T, 4> {
        {cos_r,  T(0), sin_r, T(0)},
        { T(0),  T(1),  T(0), T(0)},
        {-sin_r, T(0), cos_r, T(0)},
        { T(0),  T(0),  T(0), T(1)},
    };
}

template <typename T>
inline auto rotation_z(T radians) {
    const T cos_r = std::cos(radians);
    const T sin_r = std::sin(radians);
    return Matrix<T, 4> {
        {cos_r, -sin_r, T(0), T(0)},
        {sin_r,  cos_r, T(0), T(0)},
        { T(0),  T(0),  T(1), T(0)},
        { T(0),  T(0),  T(0), T(1)},
    };
}

template <typename T>
inline auto shearing(T xy, T xz, T yx, T yz, T zx, T zy) {
    return Matrix<T, 4> {
        {T(1),   xy,   xz, T(0)},
        { yx,  T(1),   yz, T(0)},
        { zx,    zy, T(1), T(0)},
        {T(0), T(0), T(0), T(1)},
    };
}

} // namespace rtc

#endif // RTC_LIB_TRANSFORMATIONS_H
