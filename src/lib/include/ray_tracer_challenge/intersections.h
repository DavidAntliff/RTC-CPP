#ifndef RTC_LIB_INTERSECTIONS_H
#define RTC_LIB_INTERSECTIONS_H

#include "./math.h"
#include "rays.h"
#include "shapes.h"

namespace rtc {

class Shape;

struct Intersection {
    Intersection() = default;
    Intersection(fp_t t, Shape const * object) :
            t_{t}, object_{object} {}

    auto t() const { return t_; }
    auto object() const { return object_; }

    auto operator<=>(Intersection const &) const = default;

    friend bool operator<(Intersection const& l, Intersection const& r) {
        return l.t_ < r.t_;
    }

private:
    fp_t t_ {};
    Shape const * object_ {};
};

inline auto intersection(fp_t t, Shape const & object) {
    return Intersection {t, &object};
}

using Intersections = std::vector<Intersection>;

// https://stackoverflow.com/a/75571723
// This ensures that the types are all the same.
#include <type_traits>

template <typename Head, typename... Tail>
auto intersections(Head&& head, Tail&&... tail) {
    using T = std::remove_cvref_t<Head>;
    static_assert(std::conjunction_v<std::is_same<T, std::remove_cvref_t<Tail>>...>);

    Intersections vec;
    vec.reserve(1 + sizeof...(Tail));
    vec.emplace_back(std::forward<Head>(head));
    (vec.emplace_back(std::forward<Tail>(tail)), ...);
    return vec;
}

// See also:
// https://www.scs.stanford.edu/~dm/blog/param-pack.html#homogeneous-intro

inline Intersections intersect(Shape const & shape,
                               Ray const & ray) {
    // Apply the inverse of the shape's transformation
    auto const local_ray = transform(ray, inverse(shape.transform()));

    // virtual function call
    return shape.local_intersect(local_ray);
}

inline std::optional<Intersection> hit(Intersections & intersections) {
    std::sort(intersections.begin(), intersections.end());

    auto const is_positive = [](Intersection x){ return x.t() >= 0; };
    auto const iterator = std::ranges::find_if(intersections, is_positive);

    if (iterator != intersections.end()) {
        return *iterator;
    } else {
        return {};
    }
}

struct IntersectionComputation {
    fp_t t {};
    Shape const * object {};
    Point point {};
    Point over_point {};
    Vector eyev {};
    Vector normalv {};
    bool inside {false};
};

inline auto prepare_computations(Intersection const & intersection,
                                 Ray const & ray) {
    IntersectionComputation comps {};

    comps.t = intersection.t();
    comps.object = intersection.object();

    comps.point = position(ray, comps.t);
    comps.eyev = -ray.direction();
    comps.normalv = normal_at(*comps.object, comps.point);

    if (dot(comps.normalv, comps.eyev) < 0) {
        comps.inside = true;
        comps.normalv = -comps.normalv;
    }

    comps.over_point = comps.point + comps.normalv * EPSILON;

    return comps;
}

} // namespace rtc

#endif // RTC_LIB_INTERSECTIONS_H
