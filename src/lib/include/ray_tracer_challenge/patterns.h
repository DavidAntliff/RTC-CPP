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

template <typename T>
class StripePattern : public Pattern<T> {
public:
    StripePattern() = default;

    template <typename A, typename B>
    StripePattern(A const & a, B const & b)
        : Pattern<T> {},
          a_{init_(a)},
          b_{init_(b)} {}

    ~StripePattern() = default;
    StripePattern(StripePattern const & other)
        : Pattern<T> {other},
          a_{other.a_->clone()},
          b_{other.b_->clone()} {}
    StripePattern(StripePattern &&) = default;
    StripePattern & operator=(StripePattern const & other) {
        a_ = other.a_.clone();
        b_ = other.b_.clone();
    }
    StripePattern & operator=(StripePattern &&) = default;

    friend bool operator==(StripePattern const & lhs, StripePattern const & rhs) {
        return *lhs.a_ == *rhs.a_ &&
               *lhs.b_ == *rhs.b_;
    }

    auto const & a() const { return *a_; }
    auto const & b() const { return *b_; }

    Color<T> pattern_at(Point<T> const & local_point) const override {
        if (static_cast<int>(floor(local_point.x())) % 2 == 0) {
            auto const pattern_point {inverse(a_->transform()) * local_point};
            return a_->pattern_at(pattern_point);
        }
        auto const pattern_point {inverse(b_->transform()) * local_point};
        return b_->pattern_at(pattern_point);
    }

private:
    static auto init_(Color<T> const & c) {
        return std::make_unique<SolidPattern<T>>(c);
    }

    static auto init_(Pattern<T> const & p) {
        return p.clone();
    }

protected:
    // https://stackoverflow.com/a/43263477
    virtual std::unique_ptr<Pattern<T>> clone_impl() const override {
        //return std::make_unique<StripePattern>(*this);
        auto p {std::make_unique<StripePattern>(*this)};
        p->a_ = a_->clone();
        p->b_ = b_->clone();
        return p;
    };

private:
    std::unique_ptr<Pattern<T>> a_ {};
    std::unique_ptr<Pattern<T>> b_ {};
};

template <typename A, typename B>
inline auto stripe_pattern(A const & a, B const & b) {
    return StripePattern<fp_t> {a, b};
}

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
class StripePattern2 : public NestedPatterns2<T, StripePattern2<T>> {
public:
    using NestedPatterns2<T, StripePattern2>::NestedPatterns2;

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
inline auto stripe_pattern2(A const & a, B const & b) {
    return StripePattern2<fp_t> {a, b};
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
