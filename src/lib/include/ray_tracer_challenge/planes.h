#ifndef RTC_LIB_PLANES_H
#define RTC_LIB_PLANES_H

#include "math.h"
#include "tuples.h"
#include "intersections.h"
#include "shapes.h"

namespace rtc {

class Plane;

Vector local_normal_at(Plane const & plane, Point const & local_point);

Intersections local_intersect(Plane const & plane,
                              Ray const & local_ray);

class Plane : public Shape {
public:
    Plane() = default;

    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Plane>(*this);
    }

    Intersections local_intersect(Ray const & local_ray) const override {
        return rtc::local_intersect(*this, local_ray);
    }

    Vector local_normal_at(Point const & local_point) const override {
        return rtc::local_normal_at(*this, local_point);
    }
};

inline Plane plane() {
    return {};
}

inline Vector local_normal_at(Plane const & plane, Point const & local_point) {
    (void)plane;
    (void)local_point;

    // The normal always points in the positive Y direction
    return vector(0.0, 1.0, 0.0);
}

inline Intersections local_intersect(Plane const & plane,
                                     Ray const & local_ray) {
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
