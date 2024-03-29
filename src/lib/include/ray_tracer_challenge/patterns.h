#ifndef RTC_LIB_PATTERNS_H
#define RTC_LIB_PATTERNS_H

#include "color.h"
#include "matrices.h"
#include "perlin_noise.h"

namespace rtc {

class Shape;

class Pattern {
public:
    Pattern() = default;
    virtual ~Pattern() = default;

    Pattern(Pattern &&) = default;
    Pattern& operator=(Pattern &&) = default;
    Pattern(Pattern const &) = default;
    Pattern& operator=(Pattern const &) = default;

    auto operator<=>(Pattern const &) const = default;

    virtual Color pattern_at(Point const & local_point) const = 0;

    inline Matrix<4> const & transform() const { return transform_; }
    inline void set_transform(Matrix<4> const & m) {
        transform_ = m;
    }

    // https://stackoverflow.com/a/43263477
    auto clone() const { return clone_impl(); }

protected:
    virtual std::unique_ptr<Pattern> clone_impl() const = 0;

private:
    Matrix<4> transform_ {identity4x4()};
};

inline void set_pattern_transform(Pattern & pattern, Matrix<4> const & m) {
    pattern.set_transform(m);
}

inline Color pattern_at(Pattern const & pattern, Point const & object_point) {
    // Convert object-space point to pattern-space point:
    auto const pattern_point {inverse(pattern.transform()) * object_point};
    return pattern.pattern_at(pattern_point);
}

Color pattern_at_shape(Pattern const & pattern,
                       Shape const & shape,
                       Point const & world_point);

class SolidPattern : public Pattern {
public:
    SolidPattern() = default;
    explicit SolidPattern(Color const & color)
        : c_{color} {}

    auto operator<=>(SolidPattern const &) const = default;

    Color pattern_at(Point const & local_point) const override {
        (void)local_point;
        return c_;
    }

protected:
    // https://stackoverflow.com/a/43263477
    virtual std::unique_ptr<Pattern> clone_impl() const override {
        return std::make_unique<SolidPattern>(*this);
    };

private:
    Color c_ {};
};

// CRTP Base Class for patterns that contain one sub-pattern
template <typename Derived>
class NestedPattern : public Pattern {
public:
    NestedPattern() = default;

    template <typename A>
    NestedPattern(A const & a)
            : Pattern {},
              a_{init_(a)} {}

    ~NestedPattern() = default;
    NestedPattern(NestedPattern const & other)
            : Pattern {other},
              a_{other.a_->clone()} {}
    NestedPattern(NestedPattern &&) = default;
    NestedPattern & operator=(NestedPattern const & other) {
        a_ = other.a_->clone();
    }
    NestedPattern & operator=(NestedPattern &&) = default;

    friend bool operator==(NestedPattern const & lhs, NestedPattern const & rhs) {
        return *lhs.a_ == *rhs.a_;
    }

    auto const & a() const { return *a_; }

private:
    static auto init_(Color const & c) {
        return std::make_unique<SolidPattern>(c);
    }

    static auto init_(Pattern const & p) {
        return p.clone();
    }

protected:
    // https://stackoverflow.com/a/43263477
    std::unique_ptr<Pattern> clone_impl() const override {
        auto p {std::make_unique<Derived>(*static_cast<Derived const *>(this))};
        p->a_ = a_->clone();
        return p;
    };

protected:
    std::unique_ptr<Pattern> a_ {};
};

// CRTP Base Class for patterns that contain two sub-patterns
template <typename Derived>
class NestedPatterns2 : public Pattern {
public:
    NestedPatterns2() = default;

    template <typename A, typename B>
    NestedPatterns2(A const & a, B const & b)
        : Pattern {},
          a_{init_(a)},
          b_{init_(b)} {}

    ~NestedPatterns2() = default;
    NestedPatterns2(NestedPatterns2 const & other)
        : Pattern {other},
          a_{other.a_->clone()},
          b_{other.b_->clone()} {}
    NestedPatterns2(NestedPatterns2 &&) = default;
    NestedPatterns2 & operator=(NestedPatterns2 const & other) {
        a_ = other.a_->clone();
        b_ = other.b_->clone();
    }
    NestedPatterns2 & operator=(NestedPatterns2 &&) = default;

    friend bool operator==(NestedPatterns2 const & lhs, NestedPatterns2 const & rhs) {
        return *lhs.a_ == *rhs.a_ &&
               *lhs.b_ == *rhs.b_;
    }

    auto const & a() const { return *a_; }
    auto const & b() const { return *b_; }

private:
    static auto init_(Color const & c) {
        return std::make_unique<SolidPattern>(c);
    }

    static auto init_(Pattern const & p) {
        return p.clone();
    }

protected:
    // https://stackoverflow.com/a/43263477
    std::unique_ptr<Pattern> clone_impl() const override {
        auto p {std::make_unique<Derived>(*static_cast<Derived const *>(this))};
        p->a_ = a_->clone();
        p->b_ = b_->clone();
        return p;
    };

protected:
    std::unique_ptr<Pattern> a_ {};
    std::unique_ptr<Pattern> b_ {};
};

class StripePattern : public NestedPatterns2<StripePattern> {
public:
    using NestedPatterns2<StripePattern>::NestedPatterns2;

    Color pattern_at(Point const & local_point) const override {
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
    return StripePattern {a, b};
}

class GradientPattern : public NestedPatterns2<GradientPattern> {
public:
    using NestedPatterns2<GradientPattern>::NestedPatterns2;

    Color pattern_at(Point const &local_point) const override {
        auto const pattern_point_a{inverse(this->a_->transform()) * local_point};
        auto const pattern_point_b{inverse(this->b_->transform()) * local_point};
        return color(local_point.x(),
                     this->a_->pattern_at(pattern_point_a),
                     this->b_->pattern_at(pattern_point_b));
    }
};

template <typename A, typename B>
inline auto gradient_pattern(A const & a, B const & b) {
    return GradientPattern {a, b};
}

class RingPattern : public NestedPatterns2<RingPattern> {
public:
    using NestedPatterns2<RingPattern>::NestedPatterns2;

    Color pattern_at(Point const &local_point) const override {
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
    return RingPattern {a, b};
}

class CheckersPattern : public NestedPatterns2<CheckersPattern> {
public:
    using NestedPatterns2<CheckersPattern>::NestedPatterns2;

    Color pattern_at(Point const &local_point) const override {
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
    return CheckersPattern {a, b};
}

class RadialGradientPattern : public NestedPatterns2<RadialGradientPattern> {
public:
    using NestedPatterns2<RadialGradientPattern>::NestedPatterns2;

    template <typename A, typename B>
    RadialGradientPattern(A const & a, B const & b, fp_t y_factor = 0)
            : NestedPatterns2<RadialGradientPattern> {a, b}, y_factor_{y_factor} {}

    Color pattern_at(Point const &local_point) const override {
        auto const distance {sqrt(local_point.x() * local_point.x() + y_factor_ * local_point.y() * local_point.y() + local_point.z() * local_point.z())};
        auto const pattern_point_a {inverse(this->a_->transform()) * local_point};
        auto const pattern_point_b {inverse(this->b_->transform()) * local_point};
        return color(distance,
                     this->a_->pattern_at(pattern_point_a),
                     this->b_->pattern_at(pattern_point_b));
    }

private:
    fp_t y_factor_ {0};
};

template <typename A, typename B>
inline auto radial_gradient_pattern(A const & a, B const & b, fp_t y_factor = 0) {
    return RadialGradientPattern {a, b, y_factor};
}


class BlendedPattern : public NestedPatterns2<BlendedPattern> {
public:
    using NestedPatterns2<BlendedPattern>::NestedPatterns2;

    Color pattern_at(Point const &local_point) const override {
        auto const pattern_point_a {inverse(this->a_->transform()) * local_point};
        auto const pattern_point_b {inverse(this->b_->transform()) * local_point};
        auto const color_a = this->a_->pattern_at(pattern_point_a);
        auto const color_b = this->b_->pattern_at(pattern_point_b);
        return (color_a + color_b) / 2.0;
    }
};

template <typename A, typename B>
inline auto blended_pattern(A const & a, B const & b) {
    return BlendedPattern {a, b};
}


class PerturbedPattern : public NestedPattern<PerturbedPattern> {
public:
    using NestedPattern<PerturbedPattern>::NestedPattern;

    template <typename A>
    PerturbedPattern(A const & a, fp_t scale = 0.5, int num_octaves = 1, fp_t persistence = 0.9)
            : NestedPattern<PerturbedPattern> {a},
              scale_{scale},
              num_octaves_{num_octaves},
              persistence_{persistence} {}

    Color pattern_at(Point const & local_point) const override {
        auto new_x = local_point.x() + perlin_noise_.octave_perlin(local_point.x(), local_point.y(), local_point.z(), num_octaves_, persistence_) * scale_;
        auto new_y = local_point.y() + perlin_noise_.octave_perlin(local_point.x(), local_point.y(), local_point.z() + 1.0, num_octaves_, persistence_) * scale_;
        auto new_z = local_point.z() + perlin_noise_.octave_perlin(local_point.x(), local_point.y(), local_point.z() + 2.0, num_octaves_, persistence_) * scale_;
        auto perturbed_point = point(new_x, new_y, new_z);

        auto const pattern_point_a {inverse(this->a_->transform()) * perturbed_point};
        auto const color_a = this->a_->pattern_at(pattern_point_a);
        return color_a;
    }

private:
    PerlinNoise perlin_noise_ {};

    fp_t scale_ {0.5};
    int num_octaves_ {1};
    fp_t persistence_ {0.9};
};

inline auto perturbed_pattern(Pattern const & pattern,
                              fp_t scale = 1.0,
                              int num_octaves = 1,
                              fp_t persistence = 0.9) {
    return PerturbedPattern {pattern, scale, num_octaves, persistence};
}

// TODO: Spherical Texture Mapping - Page 138



} // namespace rtc

#endif // RTC_LIB_PATTERNS_H
