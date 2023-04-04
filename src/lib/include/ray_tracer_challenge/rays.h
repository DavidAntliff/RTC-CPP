#ifndef RTC_LIB_RAYS_H
#define RTC_LIB_RAYS_H

#include <algorithm>

#include "./math.h"
#include "tuples.h"
#include "matrices.h"
#include "transformations.h"

namespace rtc {

class Ray {
public:
    Ray() = default;
    Ray(Point const & origin, Vector const & direction) :
        origin_{origin}, direction_{direction} {}

    auto operator<=>(Ray const &) const = default;

    Point origin() const { return origin_; }
    Vector direction() const { return direction_; }

private:
    Point origin_ {};
    Vector direction_ {};
};

inline auto ray(Point const & origin,
                Vector const & direction) {
    return Ray {origin, direction};
}

inline auto position(Ray const & ray, fp_t t) {
    return ray.origin() + ray.direction() * t;
}

inline Ray transform(Ray const & r, Matrix<4> const & m) {
    return { m * r.origin(), m * r.direction() };
}

} // namespace rtc

#endif // RTC_LIB_RAYS_H
