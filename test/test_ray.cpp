// Chapter 5: Ray-Sphere Intersections

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ray_tracer_challenge/ray.h>
#include <ray_tracer_challenge/tuple.h>

using namespace rtc;
using ::testing::Optional;

TEST(TestRay, creating_and_querying_a_ray) {
    auto origin = point(1.0, 2.0, 3.0);
    auto direction = vector(4.0, 5.0, 6.0);
    auto r = ray(origin, direction);
    EXPECT_EQ(r.origin(), origin);
    EXPECT_EQ(r.direction(), direction);
}

// Computing a point from a distance
TEST(TestRay, compute_a_point_from_distance) {
    auto r = ray(point(2.0, 3.0, 4.0), vector(1.0, 0.0, 0.0));
    EXPECT_EQ(position(r, 0.0), point(2.0, 3.0, 4.0));
    EXPECT_EQ(position(r, 1.0), point(3.0, 3.0, 4.0));
    EXPECT_EQ(position(r, -1.0), point(1.0, 3.0, 4.0));
    EXPECT_EQ(position(r, 2.5), point(4.5, 3.0, 4.0));
}

// A ray intersects a sphere at two points
TEST(TestRay, ray_intersects_sphere_at_two_points) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), 4.0);
    EXPECT_EQ(xs[1].t(), 6.0);
}

// A ray intersects a sphere at a tangent
TEST(TestRay, ray_intersects_a_sphere_at_a_tangent) {
    auto r = ray(point(0.0, 1.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), 5.0);
    EXPECT_EQ(xs[1].t(), 5.0);
}

// A ray misses a sphere
TEST(TestRay, ray_misses_sphere) {
    auto r = ray(point(0.0, 2.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 0);
}

// A ray originates inside a sphere
TEST(TestRay, ray_originates_inside_sphere) {
    auto r = ray(point(0.0, 0.0, 0.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), -1.0);
    EXPECT_EQ(xs[1].t(), 1.0);
}

// A sphere is behind a ray
TEST(TestRay, sphere_is_behind_ray) {
    auto r = ray(point(0.0, 0.0, 5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), -6.0);
    EXPECT_EQ(xs[1].t(), -4.0);
}

// An intersection encapsulates t and object
TEST(TestRay, intersection_encapsulates_t_and_object) {
    auto s = sphere(1);
    auto i = intersection(3.5, s);
    EXPECT_EQ(i.t(), 3.5);
    EXPECT_EQ(i.object(), &s);
}

// Aggregating intersections
TEST(TestRay, aggregating_intersections) {
    auto s = sphere(1);
    auto i1 = intersection(1.0, s);
    auto i2 = intersection(2.0, s);
    auto xs = intersections(i1, i2);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), 1.0);
    EXPECT_EQ(xs[1].t(), 2.0);
}

// Intersect sets the object on the intersection
TEST(TestRay, intersect_sets_the_object) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].object(), &s);
    EXPECT_EQ(xs[1].object(), &s);
}

// The hit, when all intersections have positive t
TEST(TestRay, the_hit_when_all_intersections_have_positive_t) {
    auto s = sphere(1);
    auto i1 = intersection(1.0, s);
    auto i2 = intersection(2.0, s);
    auto xs = intersections(i2, i1);
    auto i = hit(xs);
    EXPECT_THAT(i, Optional(i1));
}

// The hit, when some intersections have negative t
TEST(TestRay, the_hit_when_some_intersections_have_negative_t) {
    auto s = sphere(1);
    auto i1 = intersection(-1.0, s);
    auto i2 = intersection(1.0, s);
    auto xs = intersections(i2, i1);
    auto i = hit(xs);
    EXPECT_THAT(i, Optional(i2));
}

// The hit, when all intersections have negative t
TEST(TestRay, the_hit_when_all_intersections_have_negative_t) {
    auto s = sphere(1);
    auto i1 = intersection(-2.0, s);
    auto i2 = intersection(-1.0, s);
    auto xs = intersections(i2, i1);
    auto i = hit(xs);
    EXPECT_EQ(i, std::nullopt);
}

// The hit is always the lowest nonnegative intersection
TEST(TestRay, the_hit_is_always_the_lowest_non_negative) {
    auto s = sphere(1);
    auto i1 = intersection(5.0, s);
    auto i2 = intersection(7.0, s);
    auto i3 = intersection(-3.0, s);
    auto i4 = intersection(2.0, s);
    auto xs = intersections(i1, i2, i3, i4);
    auto i = hit(xs);
    EXPECT_THAT(i, Optional(i4));
}
