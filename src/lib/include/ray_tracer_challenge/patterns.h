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
inline Color<T> pattern_at(Pattern<T> const & pattern, Point<T> const & object_point) {
    // Convert object-space point to pattern-space point:
    auto const pattern_point {inverse(pattern.transform()) * object_point};
    return pattern.pattern_at(pattern_point);
}

template <typename T>
inline Color<T> pattern_at_shape(Pattern<T> const & pattern,
                                 Shape<T> const & shape,
                                 Point<T> const & world_point) {
    // Convert world-space point to object-space point:
    auto const object_point {inverse(shape.transform()) * world_point};
    return pattern_at(pattern, object_point);
}

template <typename T>
class SolidPattern : public Pattern<T> {
public:
    SolidPattern() = default;
    explicit SolidPattern(Color<T> const & color)
        : c_{color} {}

    auto operator<=>(SolidPattern const &) const = default;

    Color<T> pattern_at(Point<T> const & local_point) const override {
        (void)local_point;
        return c_;
    }

protected:
    // https://stackoverflow.com/a/43263477
    virtual std::unique_ptr<Pattern<T>> clone_impl() const override {
        return std::make_unique<SolidPattern>(*this);
    };

private:
    Color<T> c_ {};
};

// CRTP Base Class for patterns that contain two sub-patterns
template <typename T, typename Derived>
class NestedPatterns2 : public Pattern<T> {
public:
    NestedPatterns2() = default;

    template <typename A, typename B>
    NestedPatterns2(A const & a, B const & b)
        : Pattern<T> {},
          a_{init_(a)},
          b_{init_(b)} {}

    ~NestedPatterns2() = default;
    NestedPatterns2(NestedPatterns2 const & other)
        : Pattern<T> {other},
          a_{other.a_->clone()},
          b_{other.b_->clone()} {}
    NestedPatterns2(NestedPatterns2 &&) = default;
    NestedPatterns2 & operator=(NestedPatterns2 const & other) {
        a_ = other.a_.clone();
        b_ = other.b_.clone();
    }
    NestedPatterns2 & operator=(NestedPatterns2 &&) = default;

    friend bool operator==(NestedPatterns2 const & lhs, NestedPatterns2 const & rhs) {
        return *lhs.a_ == *rhs.a_ &&
               *lhs.b_ == *rhs.b_;
    }

    auto const & a() const { return *a_; }
    auto const & b() const { return *b_; }

private:
    static auto init_(Color<T> const & c) {
        return std::make_unique<SolidPattern<T>>(c);
    }

    static auto init_(Pattern<T> const & p) {
        return p.clone();
    }

protected:
    // https://stackoverflow.com/a/43263477
    std::unique_ptr<Pattern<T>> clone_impl() const override {
        auto p {std::make_unique<Derived>(*static_cast<Derived const *>(this))};
        p->a_ = a_->clone();
        p->b_ = b_->clone();
        return p;
    };

protected:
    std::unique_ptr<Pattern<T>> a_ {};
    std::unique_ptr<Pattern<T>> b_ {};
};

template <typename T>
class StripePattern : public NestedPatterns2<T, StripePattern<T>> {
public:
    using NestedPatterns2<T, StripePattern>::NestedPatterns2;

    Color<T> pattern_at(Point<T> const & local_point) const override {
        if (static_cast<int>(floor(local_point.x())) % 2 == 0) {
            auto const pattern_point{inverse(this->a_->transform()) * local_point};
            return this->a_->pattern_at(pattern_point);
        }
        auto const pattern_point{inverse(this->b_->transform()) * local_point};
        return this->b_->pattern_at(pattern_point);
    }
};

template <typename A, typename B>
inline auto stripe_pattern(A const & a, B const & b) {
    return StripePattern<fp_t> {a, b};
}

template <typename T>
class GradientPattern : public NestedPatterns2<T, GradientPattern<T>> {
public:
    using NestedPatterns2<T, GradientPattern>::NestedPatterns2;

    Color<T> pattern_at(Point<T> const &local_point) const override {
        auto const pattern_point_a{inverse(this->a_->transform()) * local_point};
        auto const pattern_point_b{inverse(this->b_->transform()) * local_point};
        return color(local_point.x(),
                     this->a_->pattern_at(pattern_point_a),
                     this->b_->pattern_at(pattern_point_b));
    }
};

template <typename A, typename B>
inline auto gradient_pattern(A const & a, B const & b) {
    return GradientPattern<fp_t> {a, b};
}

template <typename T>
class RingPattern : public NestedPatterns2<T, RingPattern<T>> {
public:
    using NestedPatterns2<T, RingPattern>::NestedPatterns2;

    Color<T> pattern_at(Point<T> const &local_point) const override {
        auto const distance {sqrt(local_point.x() * local_point.x() + local_point.z() * local_point.z())};
        if (static_cast<int>(floor(distance)) % 2 == 0) {
            auto const pattern_point_a{inverse(this->a_->transform()) * local_point};
            return this->a_->pattern_at(pattern_point_a);
        }
        auto const pattern_point_b{inverse(this->b_->transform()) * local_point};
        return this->b_->pattern_at(pattern_point_b);
    }
};

template <typename A, typename B>
inline auto ring_pattern(A const & a, B const & b) {
    return RingPattern<fp_t> {a, b};
}

template <typename T>
class CheckersPattern : public NestedPatterns2<T, CheckersPattern<T>> {
public:
    using NestedPatterns2<T, CheckersPattern>::NestedPatterns2;

    Color<T> pattern_at(Point<T> const &local_point) const override {
        auto const sum = floor(local_point.x()) +
                         floor(local_point.y()) +
                         floor(local_point.z());
        if (static_cast<int>(floor(sum)) % 2 == 0) {
            auto const pattern_point_a{inverse(this->a_->transform()) * local_point};
            return this->a_->pattern_at(pattern_point_a);
        }
        auto const pattern_point_b{inverse(this->b_->transform()) * local_point};
        return this->b_->pattern_at(pattern_point_b);
    }
};

template <typename A, typename B>
inline auto checkers_pattern(A const & a, B const & b) {
    return CheckersPattern<fp_t> {a, b};
}

template <typename T>
class RadialGradientPattern : public NestedPatterns2<T, RadialGradientPattern<T>> {
public:
    using NestedPatterns2<T, RadialGradientPattern>::NestedPatterns2;

    template <typename A, typename B>
    RadialGradientPattern(A const & a, B const & b, T y_factor = T(0))
            : NestedPatterns2<T, RadialGradientPattern> {a, b}, y_factor_{y_factor} {}

    Color<T> pattern_at(Point<T> const &local_point) const override {
        auto const distance {sqrt(local_point.x() * local_point.x() + y_factor_ * local_point.y() * local_point.y() + local_point.z() * local_point.z())};
        auto const pattern_point_a {inverse(this->a_->transform()) * local_point};
        auto const pattern_point_b {inverse(this->b_->transform()) * local_point};
        return color(distance,
                     this->a_->pattern_at(pattern_point_a),
                     this->b_->pattern_at(pattern_point_b));
    }

private:
    T y_factor_ {0};
};

template <typename A, typename B>
inline auto radial_gradient_pattern(A const & a, B const & b, fp_t y_factor = 0) {
    return RadialGradientPattern {a, b, y_factor};
}


template <typename T>
class BlendedPattern : public NestedPatterns2<T, BlendedPattern<T>> {
public:
    using NestedPatterns2<T, BlendedPattern>::NestedPatterns2;

//    template <typename A, typename B>
//    BlendedPattern(A const & a, B const & b)
//            : NestedPatterns2<T, BlendedPattern> {a, b} {}

    Color<T> pattern_at(Point<T> const &local_point) const override {
        auto const pattern_point_a {inverse(this->a_->transform()) * local_point};
        auto const pattern_point_b {inverse(this->b_->transform()) * local_point};
        auto const color_a = this->a_->pattern_at(pattern_point_a);
        auto const color_b = this->b_->pattern_at(pattern_point_b);
        return (color_a + color_b) / 2.0;
    }
};

template <typename A, typename B>
inline auto blended_pattern(A const & a, B const & b) {
    return BlendedPattern<fp_t> {a, b};
}


// TODO: Spherical Texture Mapping - Page 138



} // namespace rtc

#endif // RTC_LIB_PATTERNS_H
