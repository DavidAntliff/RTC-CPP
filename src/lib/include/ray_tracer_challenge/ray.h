#ifndef RTC_LIB_RAYS_H
#define RTC_LIB_RAYS_H

#include <algorithm>

#include "./math.h"
#include "tuple.h"
#include "matrix.h"
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

template <typename T=fp_t>
class Sphere {
public:
    using matrix_t = Matrix<T, 4>;

    Sphere() = default;
    Sphere(int id) : id_{id} {}

    matrix_t const & transform() const { return transform_; }

    void set_transform(matrix_t const & m) {
        transform_ = m;
    }

private:
    int id_ {};
    matrix_t transform_ {identity4x4()};
};

inline auto sphere(int id) {
    return Sphere {id};
}

template <typename T=fp_t>
struct Intersection {
    Intersection() = default;
    Intersection(fp_t t, Sphere<T> const * object) :
        t_{t}, object_{object} {}

    auto t() const { return t_; }
    auto object() const { return object_; }

    bool operator==(const Intersection &) const = default;
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

template <typename T, typename Ray>
inline Intersections<Intersection<T>> intersect(Sphere<T> const & sphere,
                                  Ray const & ray) {
    // Apply the inverse of the Sphere's transformation
    auto ray2 = transform(ray, inverse(sphere.transform()));

    // TODO: A more stable algorithm at:
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html

    // The vector from the sphere's centre, to the ray origin
    // Remember, the sphere is centred at the world origin
    auto sphere_to_ray = ray2.origin() - point<typename Ray::Point::value_t>(0.0, 0.0, 0.0);

    auto a = dot(ray2.direction(), ray2.direction());
    auto b = 2.0 * dot(ray2.direction(), sphere_to_ray);
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

template <typename T>
inline std::optional<Intersection<T>> hit(Intersections<Intersection<T>> & intersections) {
    std::sort(intersections.begin(), intersections.end());

    auto is_positive = [](Intersection<T> x){ return x.t() >= 0; };
    auto iterator = std::ranges::find_if(intersections, is_positive);

    if (iterator != intersections.end()) {
        return *iterator;
    } else {
        return {};
    }
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
