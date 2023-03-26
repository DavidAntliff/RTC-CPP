#ifndef RTC_LIB_PATTERNS_H
#define RTC_LIB_PATTERNS_H

#include "color.h"
#include "matrices.h"

namespace rtc {

template <typename T>
class Shape;

template <typename T>
class Pattern {
public:
    using matrix_t = Matrix<T, 4>;

    Pattern() = default;
    virtual ~Pattern() = default;

    Pattern(Pattern &&) = default;
    Pattern& operator=(Pattern &&) = default;
    Pattern(Pattern const &) = default;
    Pattern& operator=(Pattern const &) = default;

    auto operator<=>(Pattern const &) const = default;

    virtual Color<T> pattern_at(Point<T> const & local_point) const = 0;

    inline matrix_t const & transform() const { return transform_; }
    inline void set_transform(matrix_t const & m) {
        transform_ = m;
    }

private:
    matrix_t transform_ {identity4x4()};
};

template <typename T, typename Matrix>
inline void set_pattern_transform(Pattern<T> & pattern, Matrix const & m) {
    pattern.set_transform(m);
}

template <typename T>
inline Color<T> pattern_at(Pattern<T> const & pattern, Point<T> const & point) {
    return pattern.pattern_at(point);
}

template <typename T>
inline Color<T> pattern_at_shape(Pattern<T> const & pattern,
                                 Shape<T> const & shape,
                                 Point<T> const & world_point) {
    // Convert world-space point to object-space point:
    auto const object_point {inverse(shape.transform()) * world_point};
    // Convert object-space point to pattern-space point:
    auto const pattern_point {inverse(pattern.transform()) * object_point};
    return pattern_at(pattern, pattern_point);
}


template <typename T>
class StripePattern : public Pattern<T> {
public:
    using matrix_t = Matrix<T, 4>;

    StripePattern() = default;
    StripePattern(Color<T> const & a, Color<T> const & b)
        : Pattern<T> {}, a_{a}, b_{b} {}

    auto const & a() const { return a_; }
    auto const & b() const { return b_; }

    Color<T> pattern_at(Point<T> const & local_point) const override {
        if (static_cast<int>(floor(local_point.x())) % 2 == 0) {
            return a_;
        }
        return b_;
    }

private:
    Color<T> a_ {};
    Color<T> b_ {};
};

template <typename T>
inline auto stripe_pattern(Color<T> const & a, Color<T> const & b) {
    return StripePattern {a, b};
}

} // namespace rtc

#endif // RTC_LIB_PATTERNS_H
