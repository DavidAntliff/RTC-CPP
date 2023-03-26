#ifndef RTC_LIB_PATTERNS_H
#define RTC_LIB_PATTERNS_H

#include "color.h"
#include "matrices.h"

namespace rtc {

template <typename T>
class Shape;

template <typename T>
class StripePattern {
public:
    using matrix_t = Matrix<T, 4>;

    StripePattern() = default;
    StripePattern(Color<T> const & a, Color<T> const & b) : a_{a}, b_{b} {}

    auto operator<=>(StripePattern const &) const = default;

    inline matrix_t const & transform() const { return transform_; }
    inline void set_transform(matrix_t const & m) {
        transform_ = m;
    }

    auto const & a() const { return a_; }
    auto const & b() const { return b_; }

private:
    matrix_t transform_ {identity4x4()};

    Color<T> a_ {};
    Color<T> b_ {};
};


template <typename T>
inline auto stripe_pattern(Color<T> const & a, Color<T> const & b) {
    return StripePattern {a, b};
}

template <typename T, typename Matrix>
inline void set_pattern_transform(StripePattern<T> & pattern, Matrix const & m) {
    pattern.set_transform(m);
}

template <typename T>
inline Color<T> stripe_at(StripePattern<T> const & pattern, Point<T> const & point) {
    if (static_cast<int>(floor(point.x())) % 2 == 0) {
        return pattern.a();
    }
    return pattern.b();
}

template <typename T>
inline Color<T> stripe_at_object(StripePattern<T> const & pattern,
                                 Shape<T> const & object,
                                 Point<T> const & world_point) {
    // Convert world-space point to object-space point:
    auto const object_point {inverse(object.transform()) * world_point};
    // Convert object-space point to pattern-space point:
    auto const pattern_point {inverse(pattern.transform()) * object_point};
    return stripe_at(pattern, pattern_point);
}

} // namespace rtc

#endif // RTC_LIB_PATTERNS_H
