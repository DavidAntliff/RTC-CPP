// Chapter 5: Ray-Sphere Intersections

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ray_tracer_challenge/spheres.h>
#include <ray_tracer_challenge/rays.h>
#include <ray_tracer_challenge/intersections.h>

using namespace rtc;
using ::testing::Optional;

// An intersection encapsulates t and object
TEST(TestIntersections, intersection_encapsulates_t_and_object) {
    auto s = sphere(1);
    auto i = intersection(3.5, s);
    EXPECT_EQ(i.t(), 3.5);
    EXPECT_EQ(i.object(), &s);
}

// Aggregating intersections
TEST(TestIntersections, aggregating_intersections) {
    auto s = sphere(1);
    auto i1 = intersection(1.0, s);
    auto i2 = intersection(2.0, s);
    auto xs = intersections(i1, i2);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), 1.0);
    EXPECT_EQ(xs[1].t(), 2.0);
}

// The hit, when all intersections have positive t
TEST(TestIntersections, the_hit_when_all_intersections_have_positive_t) {
    auto s = sphere(1);
    auto i1 = intersection(1.0, s);
    auto i2 = intersection(2.0, s);
    auto xs = intersections(i2, i1);
    auto i = hit(xs);
    EXPECT_THAT(i, Optional(i1));
}

// The hit, when some intersections have negative t
TEST(TestIntersections, the_hit_when_some_intersections_have_negative_t) {
    auto s = sphere(1);
    auto i1 = intersection(-1.0, s);
    auto i2 = intersection(1.0, s);
    auto xs = intersections(i2, i1);
    auto i = hit(xs);
    EXPECT_THAT(i, Optional(i2));
}

// The hit, when all intersections have negative t
TEST(TestIntersections, the_hit_when_all_intersections_have_negative_t) {
    auto s = sphere(1);
    auto i1 = intersection(-2.0, s);
    auto i2 = intersection(-1.0, s);
    auto xs = intersections(i2, i1);
    auto i = hit(xs);
    EXPECT_EQ(i, std::nullopt);
}

// The hit is always the lowest nonnegative intersection
TEST(TestIntersections, the_hit_is_always_the_lowest_non_negative) {
    auto s = sphere(1);
    auto i1 = intersection(5.0, s);
    auto i2 = intersection(7.0, s);
    auto i3 = intersection(-3.0, s);
    auto i4 = intersection(2.0, s);
    auto xs = intersections(i1, i2, i3, i4);
    auto i = hit(xs);
    EXPECT_THAT(i, Optional(i4));
}

// Precomputing the state of an intersection
TEST(TestIntersections, precomputing_the_state_of_an_intersection) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto shape = sphere(1);
    auto i = intersection(4.0, shape);
    auto comps = prepare_computations(i, r);
    EXPECT_EQ(comps.t, i.t());
    EXPECT_EQ(comps.object, i.object());
    EXPECT_EQ(comps.point, point(0.0, 0.0, -1.0));
    EXPECT_EQ(comps.eyev, vector(0.0, 0.0, -1.0));
    EXPECT_EQ(comps.normalv, vector(0.0, 0.0, -1.0));
}

// The hit, when an intersection occurs on the outside
TEST(TestIntersections, the_hit_when_intersection_occurs_on_outside) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto shape = sphere(1);
    auto i = intersection(4.0, shape);
    auto comps = prepare_computations(i, r);
    EXPECT_FALSE(comps.inside);
}

// The hit, when an intersection occurs on the inside
TEST(TestIntersections, the_hit_when_intersection_occurs_on_inside) {
    auto r = ray(point(0.0, 0.0, 0.0), vector(0.0, 0.0, 1.0));
    auto shape = sphere(1);
    auto i = intersection(1.0, shape);
    auto comps = prepare_computations(i, r);
    EXPECT_EQ(comps.point, point(0.0, 0.0, 1.0));
    EXPECT_EQ(comps.eyev, vector(0.0, 0.0, -1.0));
    EXPECT_TRUE(comps.inside);
    // normal would have been (0, 0, 1), but is inverted:
    EXPECT_EQ(comps.normalv, vector(0.0, 0.0, -1.0));
}
