// Chapter 9: Planes

#include <gtest/gtest.h>

#include <numbers>

#include <ray_tracer_challenge/shapes.h>
#include <ray_tracer_challenge/transformations.h>
#include <ray_tracer_challenge/rays.h>
#include <ray_tracer_challenge/intersections.h>

using namespace rtc;

class TestShape : public Shape<double> {
public:
    TestShape() = default;

    Intersections<Intersection<fp_t>> local_intersect(Ray<double> const & local_ray) const override {
        saved_ray = local_ray;
        return {};
    }

    Vector<double> local_normal_at(Point<double> const & world_point) const override {
        return vector(world_point.x(), world_point.y(), world_point.z());
    }

    mutable Ray<double> saved_ray {};
};

TestShape test_shape() {
    return {};
}

// A shape's default transformation
TEST(TestShapes, shape_has_default_transformation) {
    auto s = test_shape();
    EXPECT_EQ(s.transform(), identity4x4());
}

// Changing a shape's transformation
TEST(TestShapes, changing_shape_transformation) {
    auto s = test_shape();
    auto t = translation(2.0, 3.0, 4.0);
    set_transform(s, t);
    EXPECT_EQ(s.transform(), t);
}

// A shape has a default material
TEST(TestShapes, shape_has_default_material) {
    auto s = test_shape();
    auto m = s.material();
    EXPECT_EQ(m, material());
}

// A shape may be assigned a material
TEST(TestShapes, shape_may_be_assigned_material) {
    auto s = test_shape();
    auto m = material();
    m.set_ambient(1.0);
    s.set_material(m);
    EXPECT_EQ(s.material(), m);
}

// Intersecting a scaled shape with a ray
TEST(TestShapes, intersecting_scaled_shape_with_ray) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = test_shape();
    set_transform(s, scaling(2.0, 2.0, 2.0));
    auto xs = intersect(s, r);
    EXPECT_EQ(s.saved_ray.origin(), point(0.0, 0.0, -2.5));
    EXPECT_EQ(s.saved_ray.direction(), vector(0.0, 0.0, 0.5));
}

// Intersecting a translated shape with a ray
TEST(TestShapes, intersecting_translated_shape_with_ray) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = test_shape();
    set_transform(s, translation(5.0, 0.0, 0.0));
    auto xs = intersect(s, r);
    EXPECT_EQ(s.saved_ray.origin(), point(-5.0, 0.0, -5.0));
    EXPECT_EQ(s.saved_ray.direction(), vector(0.0, 0.0, 1.0));
}

// Computing the normal on a translated shape
TEST(TestShapes, computing_normal_on_translated_shape) {
    auto s = test_shape();
    set_transform(s, translation(0.0, 1.0, 0.0));
    auto n = normal_at(s, point(0.0, 1.70711, -0.70711));
    EXPECT_TRUE(almost_equal(n, vector(0.0, 0.70711, -0.70711), 1e-4));
}

// Computing the normal on a transformed shape
TEST(TestShapes, computing_normal_on_transformed_shape) {
    auto s = test_shape();
    auto m = scaling(1.0, 0.5, 1.0) * rotation_z(std::numbers::pi / 5.0);
    set_transform(s, m);
    auto k = sqrt(2.0) / 2.0;
    auto n = normal_at(s, point(0.0, k, -k));
    EXPECT_TRUE(almost_equal(n, vector(0.0, 0.970143, -0.242546), 1e-4));
}
