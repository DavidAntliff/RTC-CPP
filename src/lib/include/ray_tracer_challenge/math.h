#ifndef RTC_MATH_H
#define RTC_MATH_H

namespace rtc {

// Default floating point type for calculations
using fp_t = double;

template <typename T>
inline bool almost_equal(const T & lhs, const T & rhs, T epsilon=0.00001) {
    return abs(lhs - rhs) < epsilon;
}

} // namespace rtc

#endif // RTC_MATH_H
