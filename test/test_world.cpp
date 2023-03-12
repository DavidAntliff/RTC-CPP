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
