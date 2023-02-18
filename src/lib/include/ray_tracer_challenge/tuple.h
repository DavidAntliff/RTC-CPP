#ifndef RTC_MATHS_H
#define RTC_MATHS_H

#include <ostream>
#include <cmath>

// Left-Handed Coordinate System
// TODO: investigate constexpr here
// TODO: benchmark if `const Scalar &` is faster than `Scalar`

// Implementing operators:
//  https://stackoverflow.com/a/4421719

namespace rtc {

template <typename T>
inline bool almost_equal(const T & lhs, const T & rhs, T epsilon=0.00001) {
    return abs(lhs - rhs) < epsilon;
}

template <typename T=double>
struct Tuple {
    using ValueType = T;

    Tuple() = default;
    Tuple(T x, T y, T z, T w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    bool is_point() const {
        return w == 1.0;
    }

    bool is_vector() const {
        return w == 0.0;
    }

    Tuple & operator+=(const Tuple & rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Tuple & operator-=(const Tuple & rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Tuple operator-() const {
        return Tuple {-x, -y, -z, -w};
    }

    template <typename Scalar>
    Tuple & operator*=(const Scalar & rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    template <typename Scalar>
    Tuple & operator/=(const Scalar & rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Tuple & t) {
        return os << "tuple(" << t.x
                  << ", " << t.y
                  << ", " << t.z
                  << ", " << t.w << ")";
    }

    T x, y, z, w;
};

// Free functions:

// Strict equality for floating point types
template <typename T>
inline bool operator==(const Tuple<T> lhs, const Tuple<T> & rhs) {
     return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

template <typename T>
inline bool almost_equal(const Tuple<T> lhs, const Tuple<T> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
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
    return std::sqrt(t.x * t.x
        + t.y * t.y
        + t.z * t.z
        + t.w * t.w);
}

template <typename T>
inline Tuple<T> normalize(const Tuple<T> & t)
{
    return t / magnitude(t);
}

template <typename T>
inline T dot(const Tuple<T> & a, const Tuple<T> & b) {
    return a.x * b.x
         + a.y * b.y
         + a.z * b.z
         + a.w * b.w;
}

// 3D cross-product, ignore .w
template <typename T>
inline Tuple<T> cross(const Tuple<T> & a, const Tuple<T> & b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x,
            T(0)};
}



// Factory functions:
template <typename ValueType=double, typename TupleType=Tuple<ValueType>>
inline TupleType tuple(ValueType x, ValueType y, ValueType z, ValueType w) {
    return TupleType {x, y, z, w};
}

template <typename ValueType=double, typename TupleType=Tuple<ValueType>>
inline TupleType point(ValueType x, ValueType y, ValueType z) {
    return TupleType {x, y, z, 1};
}

template <typename ValueType=double, typename TupleType=Tuple<ValueType>>
inline TupleType vector(ValueType x, ValueType y, ValueType z) {
    return TupleType {x, y, z, 0};
}



} // namespace rtc

#endif // RTC_MATHS_H
