#ifndef RTC_LIB_PLANES_H
#define RTC_LIB_PLANES_H

#include "math.h"
#include "tuples.h"
#include "intersections.h"
#include "shapes.h"

namespace rtc {

template <typename T=fp_t>
class Plane;

template <typename T>
Vector<T> local_normal_at(Plane<T> const & plane, Point<T> const & local_point);

template <typename T>
Intersections<Intersection<fp_t>> local_intersect(Plane<T> const & plane,
                                                  Ray<T> const & local_ray);

template <typename T>
class Plane : public Shape<T> {
public:
    Plane() = default;

    Intersections<Intersection<fp_t>> local_intersect(Ray<T> const & local_ray) const override {
        return rtc::local_intersect(*this, local_ray);
    }

    Vector<T> local_normal_at(Point<T> const & local_point) const override {
        return rtc::local_normal_at(*this, local_point);
    }
};

template <typename T=fp_t>
inline auto plane() {
    return Plane<T> {};
}

template <typename T>
inline Vector<T> local_normal_at(Plane<T> const & plane, Point<T> const & local_point) {
    (void)plane;
    (void)local_point;

    // The normal always points in the positive Y direction
    return vector(0.0, 1.0, 0.0);
}

template <typename T>
inline Intersections<Intersection<fp_t>> local_intersect(Plane<T> const & plane,
                                                         Ray<T> const & local_ray) {
    // The plane is at the origin, extending infinitely in both X and Z directions.
    //
    // 4 cases:
    //   1. ray parallel to plane, never intersects
    //   2. ray coplanar with plane, treat as a miss
    //   3. ray origin is above the plane
    //   4. ray origin is below the plane

    if (std::abs(local_ray.direction().y()) < EPSILON) {
        return {};
    }

    auto const t = -local_ray.origin().y() / local_ray.direction().y();
    return {{t, &plane}};
}

} // namespace rtc

#endif // RTC_LIB_PLANES_H
