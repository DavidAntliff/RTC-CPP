#ifndef RTC_LIB_TUPLES_H
#define RTC_LIB_TUPLES_H

#include <ostream>
#include <cmath>
#include <optional>

#include "math.h" // NOLINT(modernize-deprecated-headers)

// Left-Handed Coordinate System

// TODO: investigate constexpr here
// TODO: benchmark if `const Scalar &` is faster than `Scalar`

// Rules for implementing operators:
//  https://stackoverflow.com/a/4421719

// Desired style of Factory functions from this namespace:
//
// tuple()
// tuple<double>()
// tuple<float>()

namespace rtc {

constexpr int TUPLE_N = 4;

struct Tuple {

    Tuple() = default;
    Tuple(fp_t x, fp_t y, fp_t z, fp_t w) :
        x_ {x}, y_ {y}, z_{z}, w_{w} {}

    virtual ~Tuple() = default;

    Tuple(Tuple &&) = default;
    Tuple& operator=(Tuple &&) = default;
    Tuple(Tuple const &) = default;
    Tuple& operator=(Tuple const &) = default;

    auto operator<=>(Tuple const &) const = default;

    fp_t x() const { return x_; }
    fp_t y() const { return y_; }
    fp_t z() const { return z_; }
    fp_t w() const { return w_; }

    std::optional<fp_t> at(unsigned int i) const {
        if (i < TUPLE_N)
            return operator()(i);
        else
            return std::nullopt;
    }

    fp_t operator()(int i) const {
        switch (i) {
            case 0: return x_;
            case 1: return y_;
            case 2: return z_;
            case 3: return w_;
            default: return 0;
        }
    }

    void set(unsigned int i, fp_t value) {
        switch (i) {
            case 0: x_ = value; break;
            case 1: y_ = value; break;
            case 2: z_ = value; break;
            case 3: w_ = value; break;
            default: break;
        }
    }

    void set_x(fp_t value) { x_ = value; }
    void set_y(fp_t value) { y_ = value; }
    void set_z(fp_t value) { z_ = value; }
    void set_w(fp_t value) { w_ = value; }

    bool is_point() const {
        return w_ == 1.0;
    }

    bool is_vector() const {
        return w_ == 0.0;
    }

    Tuple & operator+=(Tuple const & rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        w_ += rhs.w_;
        return *this;
    }

    Tuple & operator-=(Tuple const  & rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        w_ -= rhs.w_;
        return *this;
    }

    Tuple operator-() const {
        return Tuple {-x_, -y_, -z_, -w_};
    }

    template <typename Scalar>
    Tuple & operator*=(Scalar const & rhs) {
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;
        w_ *= rhs;
        return *this;
    }

    template <typename Scalar>
    Tuple & operator/=(Scalar const & rhs) {
        x_ /= rhs;
        y_ /= rhs;
        z_ /= rhs;
        w_ /= rhs;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream & os, Tuple const  & t) {
        return os << "tuple(" << t.x_
                  << ", " << t.y_
                  << ", " << t.z_
                  << ", " << t.w_ << ")";
    }

//    // Converting constructor
//    Tuple(Color const & color) :
//        x_{color.red()},
//        y_{color.green()},
//        z_{color.blue()},
//        w_{0} {}

protected:
    fp_t x_ {};
    fp_t y_ {};
    fp_t z_ {};
    fp_t w_ {};
};

// TODO: still deciding if these are useful...
// Don't want a subclass because of speed, but maybe a static polymorphic subclass?
using Point = Tuple;
using Vector = Tuple;

// Free functions:
inline bool almost_equal(Tuple const & lhs, Tuple const & rhs, fp_t epsilon=1e-5) {
    using rtc::almost_equal;
    return almost_equal(lhs.x(), rhs.x(), epsilon)
           && almost_equal(lhs.y(), rhs.y(), epsilon)
           && almost_equal(lhs.z(), rhs.z(), epsilon)
           && almost_equal(lhs.w(), rhs.w(), epsilon);
}

inline Tuple operator+(Tuple lhs, Tuple const & rhs)
{
    lhs += rhs;
    return lhs;
}

inline Tuple operator-(Tuple lhs, Tuple const & rhs)
{
    lhs -= rhs;
    return lhs;
}

inline Tuple operator*(Tuple lhs, fp_t const & rhs)
{
    lhs *= rhs;
    return lhs;
}

inline Tuple operator*(fp_t const & lhs, Tuple rhs)
{
    return rhs * lhs;
}

inline Tuple operator/(Tuple lhs, fp_t const & rhs)
{
    return lhs /= rhs;
}

inline fp_t magnitude(Tuple const & t)
{
    return std::sqrt(t.x() * t.x()
        + t.y() * t.y()
        + t.z() * t.z()
        + t.w() * t.w());
}

inline Tuple normalize(Tuple const & t)
{
    return t / magnitude(t);
}

inline fp_t dot(Tuple const & a, Tuple const & b) {
    return a.x() * b.x()
         + a.y() * b.y()
         + a.z() * b.z()
         + a.w() * b.w();
}

// 3D cross-product, ignore .w
inline Tuple cross(Tuple const & a, Tuple const & b) {
    return {a.y() * b.z() - a.z() * b.y(),
            a.z() * b.x() - a.x() * b.z(),
            a.x() * b.y() - a.y() * b.x(),
            0};
}

inline auto reflect(Vector const & in, Vector const & normal) {
    return in - normal * 2.0 * dot(in, normal);
}


// Factory functions:
inline auto tuple(fp_t x, fp_t y, fp_t z, fp_t w) {
    return Tuple {x, y, z, w};
}

inline auto point(fp_t x, fp_t y, fp_t z) {
    return Tuple {x, y, z, 1};
}

inline auto vector(fp_t x, fp_t y, fp_t z) {
    return Tuple {x, y, z, 0};
}

} // namespace rtc

#endif // RTC_LIB_TUPLES_H
