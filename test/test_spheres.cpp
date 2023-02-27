// Chapter 5: Ray-Sphere Intersections

#include <gtest/gtest.h>

#include <ray_tracer_challenge/spheres.h>
#include <ray_tracer_challenge/rays.h>
#include <ray_tracer_challenge/intersections.h>

using namespace rtc;

// A ray intersects a sphere at two points
TEST(TestSpheres, ray_intersects_sphere_at_two_points) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), 4.0);
    EXPECT_EQ(xs[1].t(), 6.0);
}

// A ray intersects a sphere at a tangent
TEST(TestSpheres, ray_intersects_a_sphere_at_a_tangent) {
    auto r = ray(point(0.0, 1.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), 5.0);
    EXPECT_EQ(xs[1].t(), 5.0);
}

// A ray misses a sphere
TEST(TestSpheres, ray_misses_sphere) {
    auto r = ray(point(0.0, 2.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 0);
}

// A ray originates inside a sphere
TEST(TestSpheres, ray_originates_inside_sphere) {
    auto r = ray(point(0.0, 0.0, 0.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), -1.0);
    EXPECT_EQ(xs[1].t(), 1.0);
}

// A sphere is behind a ray
TEST(TestSpheres, sphere_is_behind_ray) {
    auto r = ray(point(0.0, 0.0, 5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), -6.0);
    EXPECT_EQ(xs[1].t(), -4.0);
}

// Intersect sets the object on the intersection
TEST(TestSpheres, intersect_sets_the_object) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].object(), &s);
    EXPECT_EQ(xs[1].object(), &s);
}

// A sphere's default transformation
TEST(TestSpheres, sphere_default_transformation) {
    auto s = sphere(1);
    EXPECT_EQ(s.transform(), identity4x4());
}

// Changing a sphere's transformation
TEST(TestSpheres, changing_sphere_transformation) {
    auto s = sphere(1);
    auto t = translation(2.0, 3.0, 4.0);
    set_transform(s, t);
    EXPECT_EQ(s.transform(), t);
}

// Intersecting a scaled sphere with a ray
TEST(TestSpheres, intersecting_a_scaled_sphere_with_ray) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    set_transform(s, scaling(2.0, 2.0, 2.0));
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), 3.0);
    EXPECT_EQ(xs[1].t(), 7.0);
}

// Intersecting a translated sphere with a ray
TEST(TestSpheres, intersecting_translated_sphere_with_ray) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    set_transform(s, translation(5.0, 0.0, 0.0));
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 0);
}
