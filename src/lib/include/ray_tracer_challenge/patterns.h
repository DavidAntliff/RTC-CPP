#ifndef RTC_LIB_PATTERNS_H
#define RTC_LIB_PATTERNS_H

#include "color.h"

namespace rtc {

template <typename T>
class StripePattern {
public:
    StripePattern() = default;
    StripePattern(Color<T> const & a, Color<T> const & b) : a_{a}, b_{b} {}

    auto operator<=>(StripePattern const &) const = default;

    auto const & a() const { return a_; }
    auto const & b() const { return b_; }

private:
    Color<T> a_ {};
    Color<T> b_ {};
};


template <typename T>
inline auto stripe_pattern(Color<T> const & a, Color<T> const & b) {
    return StripePattern {a, b};
}

template <typename T>
inline Color<T> stripe_at(StripePattern<T> const & pattern, Point<T> const & point) {
    if (static_cast<int>(floor(point.x())) % 2 == 0) {
        return pattern.a();
    }
    return pattern.b();
}

} // namespace rtc

#endif // RTC_LIB_PATTERNS_H
