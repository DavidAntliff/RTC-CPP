#ifndef RTC_LIB_INTERSECTIONS_H
#define RTC_LIB_INTERSECTIONS_H

#include "./math.h"
#include "rays.h"

namespace rtc {

template <typename T=fp_t>
struct Intersection {
    using value_t = T;

    Intersection() = default;
    Intersection(fp_t t, Sphere<T> const * object) :
            t_{t}, object_{object} {}

    auto t() const { return t_; }
    auto object() const { return object_; }

    auto operator<=>(Intersection const &) const = default;

    friend bool operator<(Intersection const& l, Intersection const& r) {
        return l.t_ < r.t_;
    }

private:
    fp_t t_ {};
    Sphere<T> const * object_ {};
};

template <typename T=fp_t>
inline auto intersection(fp_t t, Sphere<T> const & object) {
    return Intersection {t, &object};
}

template <typename IntersectionType>
using Intersections = std::vector<IntersectionType>;

// https://stackoverflow.com/a/75569616
// This simply forwards the parameters to std::vector
//
// May need this defined:
//template<class T> Intersection(T) -> Intersection<T>;
//
template <typename... Args>
auto intersections2(Args&&... args) {
    // use the std::vector deduction guide:
    return std::vector{std::forward<Args>(args)...};

    // or if you want `Object`s created from `args`:
    //return std::vector{Object{std::forward<Args>(args)}...};
}

// https://stackoverflow.com/a/75571723
// This ensures that the types are all the same.
#include <type_traits>

template <typename Head, typename... Tail>
auto intersections(Head&& head, Tail&&... tail) {
    using T = std::remove_cvref_t<Head>;
    static_assert(std::conjunction_v<std::is_same<T, std::remove_cvref_t<Tail>>...>);

    Intersections<T> vec;
    vec.reserve(1 + sizeof...(Tail));
    vec.emplace_back(std::forward<Head>(head));
    (vec.emplace_back(std::forward<Tail>(tail)), ...);
    return vec;
}

// See also:
// https://www.scs.stanford.edu/~dm/blog/param-pack.html#homogeneous-intro

template <typename Sphere, typename Ray>
inline Intersections<Intersection<fp_t>> intersect(Sphere const & sphere,
                                                   Ray const & ray) {
    // Apply the inverse of the Sphere's transformation
    auto const ray2 = transform(ray, inverse(sphere.transform()));

    // TODO: A more stable algorithm at:
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html

    // The vector from the sphere's centre, to the ray origin
    // Remember, the sphere is centred at the world origin
    auto const sphere_to_ray = ray2.origin() - point(0.0, 0.0, 0.0);

    auto const a = dot(ray2.direction(), ray2.direction());
    auto const b = 2.0 * dot(ray2.direction(), sphere_to_ray);
    auto const c = dot(sphere_to_ray, sphere_to_ray) - 1.0;

    auto const discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0) {
        // miss
        return {};
    }

    auto const t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
    auto const t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

    return {{t1, &sphere}, {t2, &sphere}};
}

template <typename T>
inline std::optional<Intersection<T>> hit(Intersections<Intersection<T>> & intersections) {
    std::sort(intersections.begin(), intersections.end());

    auto const is_positive = [](Intersection<T> x){ return x.t() >= 0; };
    auto const iterator = std::ranges::find_if(intersections, is_positive);

    if (iterator != intersections.end()) {
        return *iterator;
    } else {
        return {};
    }
}

template <typename T>
struct IntersectionComputation {
    T t {};
    Sphere<T> const * object {};
    Point<T> point {};
    Point<T> over_point {};
    Vector<T> eyev {};
    Vector<T> normalv {};
    bool inside {false};
};

template <typename T>
inline auto prepare_computations(Intersection<T> const & intersection,
                                 Ray<T> const & ray) {
    IntersectionComputation<T> comps {};

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
