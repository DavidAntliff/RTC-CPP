// Chapter 7: Making a Scene

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <algorithm>

#include <ray_tracer_challenge/world.h>
#include <ray_tracer_challenge/lights.h>
#include <ray_tracer_challenge/color.h>
#include <ray_tracer_challenge/transformations.h>
#include <ray_tracer_challenge/materials.h>
#include <ray_tracer_challenge/rays.h>

using namespace rtc;
using ::testing::Optional;

// Creating a world
TEST(TestWorld, creating_a_world) {
    auto w = world();
    EXPECT_EQ(w.light(), std::nullopt);
    EXPECT_TRUE(w.objects().empty());
}

// The default world
TEST(TestWorld, default_world) {
    auto light = point_light(point(-10.0, 10.0, -10.0), color(1.0, 1.0, 1.0));
    auto m = material(color(0.8, 1.0, 0.6),
                      0.1,
                      0.7,
                      0.2,
                      200.0);
    auto s1 = sphere(1);
    s1.set_material(m);
    auto s2 = sphere(2);
    s2.set_transform(scaling(0.5, 0.5, 0.5));
    auto w = default_world();
    EXPECT_THAT(w.light(), Optional(light));
    // idiom for checking if item in vector is to test for != end iterator
    EXPECT_NE(std::find(w.objects().begin(), w.objects().end(), s1), w.objects().end());
    EXPECT_NE(std::find(w.objects().begin(), w.objects().end(), s2), w.objects().end());
}

// Intersect a world with a ray
TEST(TestWorld, intersect_world_with_ray) {
    auto w = default_world();
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto xs = intersect_world(w, r);
    ASSERT_EQ(xs.size(), 4);
    EXPECT_EQ(xs[0].t(), 4.0);
    EXPECT_EQ(xs[1].t(), 4.5);
    EXPECT_EQ(xs[2].t(), 5.5);
    EXPECT_EQ(xs[3].t(), 6.0);
}

// Shading an intersection
TEST(TestWorld, shading_an_intersection) {
    auto w = default_world();
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto shape = w.objects()[0];
    auto i = intersection(4.0, shape);
    auto comps = prepare_computations(i, r);
    auto c = shade_hit(w, comps);
    EXPECT_TRUE(almost_equal(c, color(0.38066, 0.47583, 0.2855)));
}

// Shading an intersection from the inside
TEST(TestWorld, shading_an_intersection_from_inside) {
    auto w = default_world();
    w.add_light(point_light(point(0.0, 0.25, 0.0), color(1.0, 1.0, 1.0)));
    auto r = ray(point(0.0, 0.0, 0.0), vector(0.0, 0.0, 1.0));
    auto shape = w.objects()[1];
    auto i = intersection(0.5, shape);
    auto comps = prepare_computations(i, r);
    auto c = shade_hit(w, comps);
    EXPECT_TRUE(almost_equal(c, color(0.90498, 0.90498, 0.90498)));
}

// The color when a ray misses
TEST(TestWorld, color_when_ray_misses) {
    auto w = default_world();
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 1.0, 0.0));
    auto c = color_at(w, r);
    EXPECT_EQ(c, color(0.0, 0.0, 0.0));
}

// The color when a ray hits
TEST(TestWorld, color_when_ray_hits) {
    auto w = default_world();
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto c = color_at(w, r);
    EXPECT_TRUE(almost_equal(c, color(0.38066, 0.47583, 0.2855)));
}

// The color with an intersection behind the ray
TEST(TestWorld, color_with_intersection_behind_ray) {
    auto w = default_world();
    auto outer = w.objects()[0];
    outer.material().set_ambient(1.0);
    auto & inner = w.objects()[1];
    inner.material().set_ambient(1.0);
    auto r = ray(point(0.0, 0.0, 0.75), vector(0.0, 0.0, -1.0));
    auto c = color_at(w, r);
    EXPECT_EQ(c, inner.material().color());
}

// There is no shadow when nothing is collinear with point and light
TEST(TestWorld, no_shadow_when_nothing_between_point_and_light) {
    auto w = default_world();
    auto p = point(0.0, 10.0, 0.0);
    EXPECT_FALSE(is_shadowed(w, p));
}

// The shadow when an object is between the point and the light
TEST(TestWorld, shadow_when_object_between_point_and_light) {
    auto w = default_world();
    auto p = point(10.0, -10.0, 10.0);
    EXPECT_TRUE(is_shadowed(w, p));
}

// There is no shadow when an object is behind the light
TEST(TestWorld, no_shadow_when_object_is_behind_light) {
    auto w = default_world();
    auto p = point(-20.0, 20.0, -20.0);
    EXPECT_FALSE(is_shadowed(w, p));
}

// There is no shadow when an object is behind the point
TEST(TestWorld, no_shadow_when_object_is_behind_point) {
    auto w = default_world();
    auto p = point(-2.0, 2.0, -2.0);
    EXPECT_FALSE(is_shadowed(w, p));
}

// shade_hit() is given an intersection in shadow
TEST(TestWorld, shade_hit_given_intersection_in_shadow) {
    auto w = world();
    w.add_light(point_light(point(0.0, 0.0, -10.0), color(1.0, 1.0, 1.0)));
    auto s1 = sphere(1);
    w.add_object(s1);
    auto s2 = sphere(2);
    s2.set_transform(translation(0.0, 0.0, 10.0));
    w.add_object(s2);
    auto r = ray(point(0.0, 0.0, 5.0), vector(0.0, 0.0, 1.0));
    auto i = intersection(4.0, s2);
    auto comps = prepare_computations(i, r);
    auto c = shade_hit(w, comps);
    EXPECT_EQ(c, color(0.1, 0.1, 0.1));
}
