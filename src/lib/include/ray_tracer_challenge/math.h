#ifndef RTC_MATH_H
#define RTC_MATH_H

namespace rtc {

// Default floating point type for calculations
using fp_t = double;

constexpr fp_t EPSILON {1e-9};

template <typename T>
inline bool almost_equal(T const & lhs, T const & rhs, T epsilon=1e-5) {
    return abs(lhs - rhs) < epsilon;
}

} // namespace rtc

#endif // RTC_MATH_H
