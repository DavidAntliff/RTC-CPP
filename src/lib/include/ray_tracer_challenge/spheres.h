#ifndef RTC_LIB_SPHERES_H
#define RTC_LIB_SPHERES_H

#include "math.h" // NOLINT(modernize-deprecated-headers)
#include "tuples.h"
#include "matrices.h"
#include "materials.h"

namespace rtc {

template <typename T=fp_t>
class Sphere {
public:
    using value_t = T;
    using matrix_t = Matrix<T, 4>;

    Sphere() = default;
    Sphere(int id) : id_{id} {}

    auto operator<=>(Sphere const &) const = default;

    matrix_t const & transform() const { return transform_; }

    void set_transform(matrix_t const & m) {
        transform_ = m;
    }

    auto const & material() const { return material_; }
    auto & material() { return material_; }

    void set_material(Material<T> const & material) {
        material_ = material;
    }

private:
    int id_ {};
    matrix_t transform_ {identity4x4()};
    Material<T> material_ {};
};

template <typename T=fp_t>
inline auto sphere(int id) {
    return Sphere<T> {id};
}

template <typename T>
inline auto normal_at(Sphere<T> const & sphere, Point<T> const & world_point) {
    // TODO: move to member function

    // Assume the point is always on the surface of the sphere

    // Why multiply by the inverse transpose?
    // https://stackoverflow.com/questions/13654401/why-transform-normals-with-the-transpose-of-the-inverse-of-the-modelview-matrix
    auto const object_point = inverse(sphere.transform()) * world_point;
    auto const object_normal = object_point - point(0.0, 0.0, 0.0);
    auto world_normal = transpose(inverse(sphere.transform())) * object_normal;
    world_normal.set(3, 0.0);
    return normalize(world_normal);
}

} // namespace rtc

#endif // RTC_LIB_SPHERES_H
