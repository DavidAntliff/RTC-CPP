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

    // https://stackoverflow.com/a/43263477
    auto clone() const { return clone_impl(); }

protected:
    virtual std::unique_ptr<Pattern> clone_impl() const = 0;

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

protected:
    // https://stackoverflow.com/a/43263477
    virtual std::unique_ptr<Pattern<T>> clone_impl() const override {
        return std::make_unique<StripePattern>(*this);
    };

private:
    Color<T> a_ {};
    Color<T> b_ {};
};

template <typename T>
inline auto stripe_pattern(Color<T> const & a, Color<T> const & b) {
    return StripePattern {a, b};
}


template <typename T>
class GradientPattern : public Pattern<T> {
public:
    GradientPattern() = default;
    GradientPattern(Color<T> const & a, Color<T> const & b)
        : Pattern<T> {}, a_{a}, b_{b} {}

    auto const & a() const { return a_; }
    auto const & b() const { return b_; }

    Color<T> pattern_at(Point<T> const & local_point) const override {
        return color(local_point.x(), a_, b_);
    }

protected:
    // https://stackoverflow.com/a/43263477
    virtual std::unique_ptr<Pattern<T>> clone_impl() const override {
        return std::make_unique<GradientPattern>(*this);
    };

private:
    Color<T> a_ {};
    Color<T> b_ {};
};

template <typename T>
inline auto gradient_pattern(Color<T> const & a, Color<T> const & b) {
    return GradientPattern {a, b};
}

template <typename T>
class RingPattern : public Pattern<T> {
public:
    RingPattern() = default;
    RingPattern(Color<T> const & a, Color<T> const & b)
        : Pattern<T> {}, a_{a}, b_{b} {}

    auto const & a() const { return a_; }
    auto const & b() const { return b_; }

    Color<T> pattern_at(Point<T> const & local_point) const override {
        auto const distance {sqrt(local_point.x() * local_point.x() + local_point.z() * local_point.z())};
        if (static_cast<int>(floor(distance)) % 2 == 0) {
            return a_;
        }
        return b_;
    }

protected:
    // https://stackoverflow.com/a/43263477
    virtual std::unique_ptr<Pattern<T>> clone_impl() const override {
        return std::make_unique<RingPattern>(*this);
    };

private:
    Color<T> a_ {};
    Color<T> b_ {};
};

template <typename T>
inline auto ring_pattern(Color<T> const & a, Color<T> const & b) {
    return RingPattern {a, b};
}

template <typename T>
class CheckersPattern : public Pattern<T> {
public:
    CheckersPattern() = default;
    CheckersPattern(Color<T> const & a, Color<T> const & b)
            : Pattern<T> {}, a_{a}, b_{b} {}

    auto const & a() const { return a_; }
    auto const & b() const { return b_; }

    Color<T> pattern_at(Point<T> const & local_point) const override {
        auto const sum = floor(local_point.x()) +
                         floor(local_point.y()) +
                         floor(local_point.z());
        if (static_cast<int>(floor(sum)) % 2 == 0) {
            return a_;
        }
        return b_;
    }

protected:
    // https://stackoverflow.com/a/43263477
    virtual std::unique_ptr<Pattern<T>> clone_impl() const override {
        return std::make_unique<CheckersPattern>(*this);
    };

private:
    Color<T> a_ {};
    Color<T> b_ {};
};

template <typename T>
inline auto checkers_pattern(Color<T> const & a, Color<T> const & b) {
    return CheckersPattern {a, b};
}

template <typename T>
class RadialGradientPattern : public Pattern<T> {
public:
    RadialGradientPattern() = default;
    RadialGradientPattern(Color<T> const & a, Color<T> const & b, T y_factor = T(0))
        : Pattern<T> {}, a_{a}, b_{b}, y_factor_{y_factor} {}

    auto const & a() const { return a_; }
    auto const & b() const { return b_; }

    Color<T> pattern_at(Point<T> const & local_point) const override {
        auto const distance {sqrt(local_point.x() * local_point.x() + y_factor_ * local_point.y() * local_point.y() + local_point.z() * local_point.z())};
        return color(distance, a_, b_);
    }

protected:
    // https://stackoverflow.com/a/43263477
    virtual std::unique_ptr<Pattern<T>> clone_impl() const override {
        return std::make_unique<RadialGradientPattern>(*this);
    };

private:
    Color<T> a_ {};
    Color<T> b_ {};

    T y_factor_ {0};
};

template <typename T>
inline auto radial_gradient_pattern(Color<T> const & a, Color<T> const & b, T y_factor = T(0)) {
    return RadialGradientPattern {a, b, y_factor};
}

// TODO: Spherical Texture Mapping - Page 138



} // namespace rtc

#endif // RTC_LIB_PATTERNS_H
