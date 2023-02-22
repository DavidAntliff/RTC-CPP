#ifndef RTC_LIB_RAYS_H
#define RTC_LIB_RAYS_H

#include <algorithm>

#include "./math.h"
#include "tuple.h"
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


class Sphere {
public:
    Sphere() = default;
    Sphere(int id) : id_{id} {}

private:
    int id_ {};
};

inline auto sphere(int id) {
    return Sphere {id};
}


struct Intersection {
    Intersection(fp_t t, Sphere const * object) :
        t_{t}, object_{object} {}

    auto t() const { return t_; }
    auto object() const { return object_; }

    bool operator==(const Intersection &) const = default;
    friend bool operator<(Intersection const& l, Intersection const& r) {
        return l.t_ < r.t_;
    }

private:
    fp_t t_ {};
    Sphere const * object_ {};
};

using Intersections = std::vector<Intersection>;

inline auto intersection(fp_t t, Sphere const & object) {
    return Intersection {t, &object};
}

// https://stackoverflow.com/questions/59655413/can-i-list-initialize-stdvector-with-perfect-forwarding-of-the-elements
template <typename... Args>
inline Intersections intersections(Args&&... args) {
    Intersections vec;
    vec.reserve(sizeof...(Args));
    (vec.emplace_back(std::forward<Args>(args)), ...);
    return vec;
}

template<typename Ray>
inline Intersections intersect(Sphere const & sphere,
                               Ray const & ray) {
    // TODO: A more stable algorithm at:
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html

    // The vector from the sphere's centre, to the ray origin
    // Remember, the sphere is centred at the world origin
    auto sphere_to_ray = ray.origin() - point<typename Ray::Point::value_t>(0.0, 0.0, 0.0);

    auto a = dot(ray.direction(), ray.direction());
    auto b = 2.0 * dot(ray.direction(), sphere_to_ray);
    auto c = dot(sphere_to_ray, sphere_to_ray) - 1.0;

    auto discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0) {
        // miss
        return {};
    }

    auto t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
    auto t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);

    return {{t1, &sphere}, {t2, &sphere}};
}

inline std::optional<Intersection> hit(Intersections & intersections) {
    std::sort(intersections.begin(), intersections.end());

    auto is_positive = [](Intersection x){ return x.t() >= 0; };
    auto iterator = std::ranges::find_if(intersections, is_positive);

    if (iterator != intersections.end()) {
        return *iterator;
    } else {
        return {};
    }
}

} // namespace rtc

#endif // RTC_LIB_RAYS_H
