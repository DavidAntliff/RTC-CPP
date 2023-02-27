// Chapter 5: Ray-Sphere Intersections

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ray_tracer_challenge/rays.h>
#include <ray_tracer_challenge/tuples.h>

using namespace rtc;
using ::testing::Optional;

TEST(TestRays, creating_and_querying_a_ray) {
    auto origin = point(1.0, 2.0, 3.0);
    auto direction = vector(4.0, 5.0, 6.0);
    auto r = ray(origin, direction);
    EXPECT_EQ(r.origin(), origin);
    EXPECT_EQ(r.direction(), direction);
}

// Computing a point from a distance
TEST(TestRays, compute_a_point_from_distance) {
    auto r = ray(point(2.0, 3.0, 4.0), vector(1.0, 0.0, 0.0));
    EXPECT_EQ(position(r, 0.0), point(2.0, 3.0, 4.0));
    EXPECT_EQ(position(r, 1.0), point(3.0, 3.0, 4.0));
    EXPECT_EQ(position(r, -1.0), point(1.0, 3.0, 4.0));
    EXPECT_EQ(position(r, 2.5), point(4.5, 3.0, 4.0));
}

// Translating a ray
TEST(TestRays, translating_a_ray) {
    auto r = ray(point(1.0, 2.0, 3.0), vector(0.0, 1.0, 0.0));
    auto m = translation(3.0, 4.0, 5.0);
    auto r2 = transform(r, m);
    EXPECT_EQ(r2.origin(), point(4.0, 6.0, 8.0));
    EXPECT_EQ(r2.direction(), vector(0.0, 1.0, 0.0));
}

// Scaling a ray
TEST(TestRays, scaling_a_ray) {
    auto r = ray(point(1.0, 2.0, 3.0), vector(0.0, 1.0, 0.0));
    auto m = scaling(2.0, 3.0, 4.0);
    auto r2 = transform(r, m);
    EXPECT_EQ(r2.origin(), point(2.0, 6.0, 12.0));
    EXPECT_EQ(r2.direction(), vector(0.0, 3.0, 0.0));
}
