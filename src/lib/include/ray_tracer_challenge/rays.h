#ifndef RTC_LIB_RAYS_H
#define RTC_LIB_RAYS_H

#include <algorithm>

#include "./math.h"
#include "tuples.h"
#include "matrices.h"
#include "transformations.h"

namespace rtc {

template <typename T=fp_t>
class Ray {
public:
    using value_t = T;

    Ray() = default;
    Ray(Point<T> const & origin, Vector<T> const & direction) :
        origin_{origin}, direction_{direction} {}

    auto operator<=>(Ray const &) const = default;

    Point<T> origin() const { return origin_; }
    Vector<T> direction() const { return direction_; }

private:
    Point<T> origin_ {};
    Vector<T> direction_ {};
};

template <typename T>
inline auto ray(Point<T> const & origin,
                Vector<T> const & direction) {
    return Ray<T>{origin, direction};
}

template <typename T>
inline auto position(Ray<T> const & ray, T t) {
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
