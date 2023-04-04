#ifndef RTC_LIB_TRANSFORMATIONS_H
#define RTC_LIB_TRANSFORMATIONS_H

#include <cmath>

#include "tuples.h"
#include "matrices.h"

namespace rtc {

inline auto translation(fp_t x, fp_t y, fp_t z) {
    return Matrix<4> {
        {1, 0, 0, x},
        {0, 1, 0, y},
        {0, 0, 1, z},
        {0, 0, 0, 1},
    };
}

inline auto scaling(fp_t x, fp_t y, fp_t z) {
    return Matrix<4> {
        {x, 0, 0, 0},
        {0, y, 0, 0},
        {0, 0, z, 0},
        {0, 0, 0, 1},
    };
}

inline auto rotation_x(fp_t radians) {
    auto const cos_r = std::cos(radians);
    auto const sin_r = std::sin(radians);
    return Matrix<4> {
        {1,     0,      0, 0},
        {0, cos_r, -sin_r, 0},
        {0, sin_r,  cos_r, 0},
        {0,     0,      0, 1},
    };
}

inline auto rotation_y(fp_t radians) {
    auto const cos_r = std::cos(radians);
    auto const sin_r = std::sin(radians);
    return Matrix<4> {
        { cos_r,  0, sin_r, 0},
        {     0,  1,     0, 0},
        {-sin_r,  0, cos_r, 0},
        {     0,  0,     0, 1},
    };
}

inline auto rotation_z(fp_t radians) {
    auto const cos_r = std::cos(radians);
    auto const sin_r = std::sin(radians);
    return Matrix<4> {
        {cos_r, -sin_r, 0, 0},
        {sin_r,  cos_r, 0, 0},
        {    0,      0, 1, 0},
        {    0,      0, 0, 1},
    };
}

inline auto shearing(fp_t xy, fp_t xz, fp_t yx, fp_t yz, fp_t zx, fp_t zy) {
    return Matrix<4> {
        { 1, xy, xz, 0},
        {yx,  1, yz, 0},
        {zx, zy,  1, 0},
        { 0,  0,  0, 1},
    };
}

inline auto view_transform(Point const & from,
                           Point const & to,
                           Vector const & up) {
    auto const forward = normalize(to - from);
    auto const upn = normalize(up);
    auto const left = cross(forward, upn);
    auto const true_up = cross(left, forward);

    Matrix<4> const orientation {
        {    left.x(),     left.y(),     left.z(), 0},
        { true_up.x(),  true_up.y(),  true_up.z(), 0},
        {-forward.x(), -forward.y(), -forward.z(), 0},
        {           0,            0,            0, 1},
    };

    return orientation * translation(-from.x(), -from.y(), -from.z());
}

} // namespace rtc

#endif // RTC_LIB_TRANSFORMATIONS_H
