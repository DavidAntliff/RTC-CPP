#ifndef RTC_LIB_SPHERES_H
#define RTC_LIB_SPHERES_H

#include "math.h" // NOLINT(modernize-deprecated-headers)
#include "tuples.h"
#include "matrices.h"
#include "intersections.h"
#include "shapes.h"

namespace rtc {

class Sphere;

Vector local_normal_at(Sphere const & sphere, Point const & local_point);

Intersections local_intersect(Sphere const & sphere,
                              Ray const & local_ray);

class Sphere : public Shape {
public:
    Sphere() = default;
    explicit Sphere(int id) : id_{id} {}

    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Sphere>(*this);
    }

    Intersections local_intersect(Ray const & local_ray) const override {
        return rtc::local_intersect(*this, local_ray);
    }

    Vector local_normal_at(Point const & local_point) const override {
        return rtc::local_normal_at(*this, local_point);
    }

private:
    int id_ {};
};

inline Sphere sphere(int id) {
    return Sphere {id};
}

inline Vector local_normal_at(Sphere const & sphere, Point const & local_point) {
    (void)sphere;
    // Assume the point is always on the surface of the sphere
    auto object_normal = local_point - point(0.0, 0.0, 0.0);
    object_normal.set_w(0);
    return normalize(object_normal);
}

inline Intersections local_intersect(Sphere const & sphere,
                                     Ray const & local_ray) {
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
