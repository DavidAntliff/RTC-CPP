#ifndef RTC_LIB_SHAPES_H
#define RTC_LIB_SHAPES_H

#include "matrices.h"
#include "intersections.h"
#include "materials.h"

namespace rtc {

template <typename T>
class Shape {
public:
    using value_t = T;
    using matrix_t = Matrix<T, 4>;

    Shape() = default;
    virtual ~Shape() = default;

    Shape(Shape &&) = default;
    Shape& operator=(Shape &&) = default;
    Shape(Shape const &) = default;
    Shape& operator=(Shape const &) = default;

    virtual Intersections<Intersection<fp_t>> local_intersect(Ray<T> const & local_ray) const = 0;

    virtual Vector<T> local_normal_at(Point<T> const & world_point) const = 0;

    auto operator<=>(Shape const &) const = default;

    inline matrix_t const & transform() const { return transform_; }
    inline void set_transform(matrix_t const & m) {
        transform_ = m;
    }

    inline auto const & material() const { return material_; }
    inline auto & material() { return material_; }

    inline void set_material(Material<T> const & material) {
        material_ = material;
    }

private:
    matrix_t transform_ {identity4x4()};
    Material<T> material_ {};
};

template <typename T, typename Matrix>
inline void set_transform(Shape<T> & shape, Matrix const & m) {
    shape.set_transform(m);
}

template <typename T>
inline auto normal_at(Shape<T> const & shape, Point<T> const & world_point) {
    // Why multiply by the inverse transpose?
    // https://stackoverflow.com/questions/13654401/why-transform-normals-with-the-transpose-of-the-inverse-of-the-modelview-matrix
    auto const local_point {inverse(shape.transform()) * world_point};
    // virtual function call
    auto const local_normal {shape.local_normal_at(local_point)};
    auto world_normal {transpose(inverse(shape.transform())) * local_normal};
    world_normal.set_w(0);
    return normalize(world_normal);
}

} // namespace rtc

#endif // RTC_LIB_SHAPES_H
