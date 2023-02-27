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

// Abstract base class, with special methods re-enabled, after
// providing the recommended virtual destructor.
struct TupleBase {
    TupleBase() = default;
    virtual ~TupleBase() = default;
    TupleBase(TupleBase &&) = default;
    TupleBase& operator=(TupleBase &&) = default;
    TupleBase(const TupleBase&) = default;
    TupleBase& operator=(const TupleBase&) = default;

    bool operator==(TupleBase const &) const = default;
};

template <typename T=fp_t, unsigned int N=4>
struct Tuple : public TupleBase {
    using value_t = T;

    Tuple() = default;
    Tuple(T x, T y, T z, T w) :
        TupleBase{},
        x_ {x}, y_ {y}, z_{z}, w_{w} {}

    virtual ~Tuple() = default;

    T x() const { return x_; }
    T y() const { return y_; }
    T z() const { return z_; }
    T w() const { return w_; }

    std::optional<T> at(unsigned int i) const {
        if (i < N)
            return operator()(i);
        else
            return std::nullopt;
    }

    T operator()(int i) const {
        switch (i) {
            case 0: return x_;
            case 1: return y_;
            case 2: return z_;
            case 3: return w_;
            default: return T(0);
        }
    }

    void set(unsigned int i, T value) {
        switch (i) {
            case 0: x_ = value; break;
            case 1: y_ = value; break;
            case 2: z_ = value; break;
            case 3: w_ = value; break;
            default: break;
        }
    }

    bool is_point() const {
        return w_ == 1.0;
    }

    bool is_vector() const {
        return w_ == 0.0;
    }

    bool operator==(Tuple const &) const = default;

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

//    // Converting constructor
//    Tuple(const Color<T> & color) :
//        x_{color.red()},
//        y_{color.green()},
//        z_{color.blue()},
//        w_{0} {}

protected:
    T x_ {};
    T y_ {};
    T z_ {};
    T w_ {};
};

// TODO: still deciding if these are useful...
// Don't want a subclass because of speed, but maybe a static polymorphic subclass?
template <typename T=fp_t>
using Point = Tuple<T>;

template <typename T=fp_t>
using Vector = Tuple<T>;

//using Point = Tuple<>;
//using Vector = Tuple<>;

// Free functions:

template <typename T>
inline bool almost_equal(const Tuple<T> & lhs, const Tuple<T> & rhs, T epsilon=1e-5) {
    using rtc::almost_equal;
    return almost_equal(lhs.x(), rhs.x(), epsilon)
           && almost_equal(lhs.y(), rhs.y(), epsilon)
           && almost_equal(lhs.z(), rhs.z(), epsilon)
           && almost_equal(lhs.w(), rhs.w(), epsilon);
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

template <typename T>
inline Tuple<T> operator*(Tuple<T> lhs, const T & rhs)
{
    lhs *= rhs;
    return lhs;
}

template <typename T>
inline Tuple<T> operator*(const T & lhs, Tuple<T> rhs)
{
    return rhs * lhs;
}

template <typename T>
inline Tuple<T> operator/(Tuple<T> lhs, const T & rhs)
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

template <typename T>
inline auto reflect(Vector<T> const & in, Vector<T> const & normal) {
    return in - normal * T(2) * dot(in, normal);
}


// Factory functions:
template <typename T=fp_t>
inline auto tuple(T x, T y, T z, T w) {
    return Tuple<T> {x, y, z, w};
}

template <typename T=fp_t>
inline auto point(T x, T y, T z) {
    return Tuple<T> {x, y, z, 1};
}

template <typename T=fp_t>
inline auto vector(T x, T y, T z) {
    return Tuple<T> {x, y, z, 0};
}

} // namespace rtc

#endif // RTC_LIB_TUPLES_H
