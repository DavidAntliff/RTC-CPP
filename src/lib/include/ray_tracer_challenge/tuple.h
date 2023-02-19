#ifndef RTC_LIB_TUPLE_H
#define RTC_LIB_TUPLE_H

#include <ostream>
#include <cmath>

#include "math.h"

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

namespace rtc::default_tuple {

using default_t = double;

template <typename T=default_t>
struct Tuple {
    using value_t = T;

    Tuple() = default;
    Tuple(T x, T y, T z, T w) {
        x_ = x;
        y_ = y;
        z_ = z;
        w_ = w;
    }

    T x() const { return x_; }
    T y() const { return y_; }
    T z() const { return z_; }
    T w() const { return w_; }

    bool is_point() const {
        return w_ == 1.0;
    }

    bool is_vector() const {
        return w_ == 0.0;
    }

    Tuple & operator+=(const Tuple & rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        w_ += rhs.w_;
        return *this;
    }

    Tuple & operator-=(const Tuple & rhs) {
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
    Tuple & operator*=(const Scalar & rhs) {
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;
        w_ *= rhs;
        return *this;
    }

    template <typename Scalar>
    Tuple & operator/=(const Scalar & rhs) {
        x_ /= rhs;
        y_ /= rhs;
        z_ /= rhs;
        w_ /= rhs;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Tuple & t) {
        return os << "tuple(" << t.x_
                  << ", " << t.y_
                  << ", " << t.z_
                  << ", " << t.w_ << ")";
    }


protected:
    T x_, y_, z_, w_;
};

// TODO: still deciding if these are useful...
//template <typename T=default_t>
//using Point = Tuple<T>;
//
//template <typename T=default_t>
//using Vector = Tuple<T>;
using Point = Tuple<>;
using Vector = Tuple<>;

// Free functions:

// Strict equality for floating point types
template <typename T>
inline bool operator==(const Tuple<T> & lhs, const Tuple<T> & rhs) {
     return lhs.x() == rhs.x()
         && lhs.y() == rhs.y()
         && lhs.z() == rhs.z()
         && lhs.w() == rhs.w();
}

template <typename T>
inline bool almost_equal(const Tuple<T> & lhs, const Tuple<T> & rhs) {
    using rtc::almost_equal;
    return almost_equal(lhs.x(), rhs.x())
           && almost_equal(lhs.y(), rhs.y())
           && almost_equal(lhs.z(), rhs.z())
           && almost_equal(lhs.w(), rhs.w());
}

template <typename T>
inline Tuple<T> operator+(Tuple<T> lhs, const Tuple<T> & rhs)
{
    lhs += rhs;
    return lhs;
}

template <typename T>
inline Tuple<T> operator-(Tuple<T> lhs, const Tuple<T> & rhs)
{
    lhs -= rhs;
    return lhs;
}

template <typename T, typename Scalar>
inline Tuple<T> operator*(Tuple<T> lhs, const Scalar & rhs)
{
    lhs *= rhs;
    return lhs;
}

template <typename T, typename Scalar>
inline Tuple<T> operator*(const Scalar & lhs, Tuple<T> rhs)
{
    return rhs * lhs;
}

template <typename T, typename Scalar>
inline Tuple<T> operator/(Tuple<T> lhs, const Scalar & rhs)
{
    return lhs /= rhs;
}

template <typename T>
inline T magnitude(const Tuple<T> & t)
{
    return std::sqrt(t.x() * t.x()
        + t.y() * t.y()
        + t.z() * t.z()
        + t.w() * t.w());
}

template <typename T>
inline Tuple<T> normalize(const Tuple<T> & t)
{
    return t / magnitude(t);
}

template <typename T>
inline T dot(const Tuple<T> & a, const Tuple<T> & b) {
    return a.x() * b.x()
         + a.y() * b.y()
         + a.z() * b.z()
         + a.w() * b.w();
}

// 3D cross-product, ignore .w
template <typename T>
inline Tuple<T> cross(const Tuple<T> & a, const Tuple<T> & b) {
    return {a.y() * b.z() - a.z() * b.y(),
            a.z() * b.x() - a.x() * b.z(),
            a.x() * b.y() - a.y() * b.x(),
            T(0)};
}


// Factory functions:
template <typename T=default_t>
inline Tuple<T> tuple(T x, T y, T z, T w) {
    return Tuple<T> {x, y, z, w};
}

template <typename T=default_t>
inline Tuple<T> point(T x, T y, T z) {
    return Tuple<T> {x, y, z, 1};
}

template <typename T=default_t>
inline Tuple<T> vector(T x, T y, T z) {
    return Tuple<T> {x, y, z, 0};
}

} // namespace rtc::default_tuple

#endif // RTC_LIB_TUPLE_H
