#ifndef RTC_LIB_SHAPES_H
#define RTC_LIB_SHAPES_H

#include "matrices.h"
#include "materials.h"
#include "rays.h"

namespace rtc {

// Forward
class Intersection;
using Intersections = std::vector<Intersection>;

class Shape {
public:
    Shape() = default;
    virtual ~Shape() = default;

    Shape(Shape &&) = default;
    Shape& operator=(Shape &&) = default;
    Shape(Shape const &) = default;
    Shape& operator=(Shape const &) = default;

    // https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/
    // Just using 'clone' fails for multiple-inheritance, but deal with that another time
    virtual std::unique_ptr<Shape> clone() const = 0;

    virtual Intersections local_intersect(Ray const & local_ray) const = 0;

    virtual Vector local_normal_at(Point const & local_point) const = 0;

    auto operator<=>(Shape const &) const = default;

    Matrix<4> const & transform() const { return transform_; }
    void set_transform(Matrix<4> const & m) {
        transform_ = m;
    }

    auto const & material() const { return material_; }
    auto & material() { return material_; }

    void set_material(Material const & material) {
        material_ = material;
    }

private:
    Matrix<4> transform_ {identity4x4()};
    Material material_ {};
};

inline void set_transform(Shape & shape, Matrix<4> const & m) {
    shape.set_transform(m);
}

inline auto normal_at(Shape const & shape, Point const & world_point) {
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
