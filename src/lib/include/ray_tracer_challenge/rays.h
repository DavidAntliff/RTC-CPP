#ifndef RTC_LIB_RAYS_H
#define RTC_LIB_RAYS_H

#include <algorithm>

#include "./math.h"
#include "tuples.h"
#include "matrices.h"
#include "transformations.h"

namespace rtc {

template <typename TupleType>
class Ray {
public:
    using Point = TupleType;
    using Vector = TupleType;

    Ray() = default;
    Ray(const Point & origin, const Vector & direction) :
        origin_{origin}, direction_{direction} {}

    Point origin() const { return origin_; }
    Vector direction() const { return direction_; }

private:
    Point origin_ {};
    Vector direction_ {};
};

template<typename T>
inline auto ray(T const &origin,
                T const &direction) {
    return Ray<T>{origin, direction};
}

template<typename Ray>
inline auto position(Ray const &ray,
                     typename Ray::Point::value_t t) {
    return ray.origin() + ray.direction() * t;
}



template <typename Ray, typename Matrix>
inline Ray transform(Ray const & r, Matrix const & m) {
    return { m * r.origin(), m * r.direction() };
}

template <typename Shape, typename Matrix>
inline void set_transform(Shape & shape, Matrix const & m) {
    shape.set_transform(m);
}

} // namespace rtc

#endif // RTC_LIB_RAYS_H
