// Chapter 9 - Planes

#include <gtest/gtest.h>

#include <ray_tracer_challenge/planes.h>

using namespace rtc;

// The normal of a plane is constant everywhere
TEST(TestPlanes, normal_of_plane_is_constant_everywhere) {
    auto p = plane();
    auto n1 = local_normal_at(p, point(0.0, 0.0, 0.0));
    auto n2 = local_normal_at(p, point(10.0, 0.0, -10.0));
    auto n3 = local_normal_at(p, point(-5.0, 0.0, 150.0));
    EXPECT_EQ(n1, vector(0.0, 1.0, 0.0));
    EXPECT_EQ(n2, vector(0.0, 1.0, 0.0));
    EXPECT_EQ(n3, vector(0.0, 1.0, 0.0));
}

// Intersect with a ray parallel to the plane
TEST(TestPlanes, intersect_with_ray_parallel_to_plane) {
    auto p = plane();
    auto r = ray(point(0.0, 1.0, 0.0), vector(0.0, 0.0, 1.0));
    auto xs = local_intersect(p, r);
    EXPECT_TRUE(xs.empty());
}

// Intersect with a coplanar ray
TEST(TestPlanes, intersect_with_coplanar_ray) {
    auto p = plane();
    auto r = ray(point(0.0, 0.0, 0.0), vector(0.0, 0.0, 1.0));
    auto xs = local_intersect(p, r);
    EXPECT_TRUE(xs.empty());
}

// A ray intersecting a plane from above
TEST(TestPlanes, ray_intersecting_plane_from_above) {
    auto p = plane();
    auto r = ray(point(0.0, 1.0, 0.0), vector(0.0, -1.0, 0.0));
    auto xs = local_intersect(p, r);
    EXPECT_EQ(xs.size(), 1);
    EXPECT_EQ(xs[0].t(), 1.0);
    EXPECT_EQ(xs[0].object(), &p);
}

// A ray intersecting a plane from below
TEST(TestPlanes, ray_intersecting_plane_from_below) {
    auto p = plane();
    auto r = ray(point(0.0, -1.0, 0.0), vector(0.0, 1.0, 0.0));
    auto xs = local_intersect(p, r);
    EXPECT_EQ(xs.size(), 1);
    EXPECT_EQ(xs[0].t(), 1.0);
    EXPECT_EQ(xs[0].object(), &p);
}
