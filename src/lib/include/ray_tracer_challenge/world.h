#ifndef RTC_LIB_WORLD_H
#define RTC_LIB_WORLD_H

#include <memory>

#include "lights.h"
#include "shapes.h"
#include "spheres.h"
#include "lights.h"
#include "transformations.h"
#include "intersections.h"

namespace rtc {

template <typename T=fp_t>
class World {
public:
    using value_t = T;

    World() = default;

    auto & light() const {
        return light_;
    }

    // TODO: add multiple lights
    void add_light(PointLight<T> const & light) {
        light_ = light;
    }

    // TODO: encapsulate collection of objects (i.e. hide that it's a vector of unique_ptrs)
    auto const & objects() const {
        return objects_;
    }

    auto & objects() {
        return objects_;
    }

    Shape<T> * get_object(unsigned int i) {
        if (i < objects_.size()) {
            return objects_[i].get();
        }
        return nullptr;
    }

    void add_object(Shape<T> const & shape) {
        objects_.push_back(shape.clone());
    }

private:
    std::optional<PointLight<T>> light_;
    std::vector<std::unique_ptr<Shape<T>>> objects_;
};


template <typename T=fp_t>
inline auto world() {
    return World<T> {};
}

template <typename T=fp_t>
inline auto default_world() {
    auto dw = World<T> {};
    dw.add_light(point_light(point(-10.0, 10.0, -10.0), color(1.0, 1.0, 1.0)));

    auto m = material(color(0.8, 1.0, 0.6),
                      0.1,
                      0.7,
                      0.2,
                      200.0);
    auto s1 = sphere(1);
    s1.set_material(m);
    dw.add_object(s1);

    auto s2 = sphere(2);
    s2.set_transform(scaling(0.5, 0.5, 0.5));
    dw.add_object(s2);

    return dw;
}

template <typename T, typename Ray>
inline Intersections<Intersection<T>> intersect_world(World<T> const & world,
                                                      Ray const & ray) {

    Intersections<Intersection<T>> result {};
    result.reserve(2); // ~5% faster than no reserve

    // Intersections must be in sorted order
    for (auto const & obj: world.objects()) {
        auto const xs = intersect(*obj, ray);
        for (auto const & i: xs) {
            result.insert(
                    std::upper_bound(result.begin(), result.end(), i),
                    i);
        }
    }

    return result;
}

// Returns the color at the intersection encapsulated by comps, in the given world.
template <typename T>
inline auto shade_hit(World<T> const & world, IntersectionComputation<T> const & comps) {
    auto const shadowed {is_shadowed(world, comps.over_point)};
    return lighting(comps.object->material(),
                    *comps.object,
                    *world.light(),
                    comps.over_point,  // avoid boundary issues
                    comps.eyev,
                    comps.normalv,
                    shadowed);
}

template <typename T>
inline Color<T> color_at(World<T> const & world, Ray<T> const & ray) {
    auto xs = intersect_world(world, ray);
    auto const i = hit(xs);
    if (i) {
        auto const comps = prepare_computations(*i, ray);
        return shade_hit(world, comps);
    } else {
        return Color(0.0, 0.0, 0.0);
    }
}

template <typename T>
inline bool is_shadowed(World<T> const & world, Point<T> const & point) {
    if (!world.light()) return true;  // everything is in shadow

    auto const v = (*world.light()).position() - point;
    auto const distance = magnitude(v);
    auto const direction = normalize(v);

    auto const ray = Ray<T>(point, direction);
    auto intersections = intersect_world(world, ray);

    auto const h = hit(intersections);
    return (h && (*h).t() < distance);
}

} // namespace rtc

#endif // RTC_LIB_WORLD_H
