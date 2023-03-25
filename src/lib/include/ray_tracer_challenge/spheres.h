#ifndef RTC_LIB_SPHERES_H
#define RTC_LIB_SPHERES_H

#include "math.h" // NOLINT(modernize-deprecated-headers)
#include "tuples.h"
#include "matrices.h"
#include "intersections.h"
#include "shapes.h"

namespace rtc {

template <typename T=fp_t>
class Sphere;

template <typename T>
Vector<T> local_normal_at(Sphere<T> const & sphere, Point<T> const & local_point);

template <typename T>
Intersections<Intersection<fp_t>> local_intersect(Sphere<T> const & sphere,
                                                  Ray<T> const & local_ray);

template <typename T>
class Sphere : public Shape<T> {
public:
    Sphere() = default;
    Sphere(int id) : id_{id} {}

    Intersections<Intersection<fp_t>> local_intersect(Ray<T> const & local_ray) const override {
        return rtc::local_intersect(*this, local_ray);
    }

    Vector<T> local_normal_at(Point<T> const & local_point) const override {
        return rtc::local_normal_at(*this, local_point);
    }

private:
    int id_ {};
};

template <typename T=fp_t>
inline auto sphere(int id) {
    return Sphere<T> {id};
}

template <typename T>
inline Vector<T> local_normal_at(Sphere<T> const & sphere, Point<T> const & local_point) {
    (void)sphere;
    // Assume the point is always on the surface of the sphere
    auto object_normal = local_point - point(0.0, 0.0, 0.0);
    object_normal.set_w(0);
    return normalize(object_normal);
}

template <typename T>
inline Intersections<Intersection<fp_t>> local_intersect(Sphere<T> const & sphere,
                                                         Ray<T> const & local_ray) {
    // TODO: A more stable algorithm at:
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html

    // The vector from the sphere's centre, to the ray origin
    // Remember, the sphere is centred at the world origin
    auto const sphere_to_ray = local_ray.origin() - point(0.0, 0.0, 0.0);

    auto const a = dot(local_ray.direction(), local_ray.direction());
    auto const b = 2.0 * dot(local_ray.direction(), sphere_to_ray);
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

} // namespace rtc

#endif // RTC_LIB_SPHERES_H
