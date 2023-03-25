// Chapter 5: Ray-Sphere Intersections

#include <gtest/gtest.h>

#include <ray_tracer_challenge/spheres.h>
#include <ray_tracer_challenge/rays.h>

using namespace rtc;

// A ray intersects a sphere at two points
TEST(TestSpheres, ray_intersects_sphere_at_two_points) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = local_intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), 4.0);
    EXPECT_EQ(xs[1].t(), 6.0);
}

// A ray intersects a sphere at a tangent
TEST(TestSpheres, ray_intersects_a_sphere_at_a_tangent) {
    auto r = ray(point(0.0, 1.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = local_intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), 5.0);
    EXPECT_EQ(xs[1].t(), 5.0);
}

// A ray misses a sphere
TEST(TestSpheres, ray_misses_sphere) {
    auto r = ray(point(0.0, 2.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = local_intersect(s, r);
    EXPECT_EQ(xs.size(), 0);
}

// A ray originates inside a sphere
TEST(TestSpheres, ray_originates_inside_sphere) {
    auto r = ray(point(0.0, 0.0, 0.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = local_intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), -1.0);
    EXPECT_EQ(xs[1].t(), 1.0);
}

// A sphere is behind a ray
TEST(TestSpheres, sphere_is_behind_ray) {
    auto r = ray(point(0.0, 0.0, 5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = local_intersect(s, r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_EQ(xs[0].t(), -6.0);
    EXPECT_EQ(xs[1].t(), -4.0);
}

// Intersect sets the object on the intersection
TEST(TestSpheres, intersect_sets_the_object) {
    auto r = ray(point(0.0, 0.0, -5.0), vector(0.0, 0.0, 1.0));
    auto s = sphere(1);
    auto xs = local_intersect(s, r);
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

// Chapter 6 - Light and Shading

// The normal on a sphere at a point on the x axis
TEST(TestSpheres, normal_on_sphere_at_point_on_x_axis) {
    auto s = sphere(1);
    auto n = local_normal_at(s, point(1.0, 0.0, 0.0));
    EXPECT_EQ(n, vector(1.0, 0.0, 0.0));
}

// The normal on a sphere at a point on the y axis
TEST(TestSpheres, normal_on_sphere_at_point_on_y_axis) {
    auto s = sphere(1);
    auto n = local_normal_at(s, point(0.0, 1.0, 0.0));
    EXPECT_EQ(n, vector(0.0, 1.0, 0.0));
}

// The normal on a sphere at a point on the z axis
TEST(TestSpheres, normal_on_sphere_at_point_on_z_axis) {
    auto s = sphere(1);
    auto n = local_normal_at(s, point(0.0, 0.0, 1.0));
    EXPECT_EQ(n, vector(0.0, 0.0, 1.0));
}

// The normal on a sphere at a non-axial point
TEST(TestSpheres, normal_on_sphere_at_non_axial_point) {
    auto s = sphere(1);
    auto k = sqrt(3.0) / 3.0;
    auto n = local_normal_at(s, point(k, k, k));
    EXPECT_EQ(n, vector(k, k, k));
}

// The normal is a normalized vector
TEST(TestSphere, normal_is_normalized_vector) {
    auto s = sphere(1);
    auto k = sqrt(3.0) / 3.0;
    auto n = local_normal_at(s, point(k, k, k));
    EXPECT_EQ(n, normalize(n));
}

// Computing the normal on a translated sphere
TEST(TestSphere, compute_normal_on_translated_sphere) {
    auto s = sphere(1);
    set_transform(s, translation(0.0, 1.0, 0.0));
    auto n = normal_at(s, point(0.0, 1.70711, -0.70711));
    EXPECT_TRUE(almost_equal(n, vector(0.0, 0.70711, -0.70711), 1e-4));
}

// Computing the normal on a transformed sphere
TEST(TestSphere, compute_normal_on_transformed_sphere) {
    auto s = sphere(1);
    auto m = scaling(1.0, 0.5, 1.0) * rotation_z(std::numbers::pi / 5.0);
    set_transform(s, m);
    auto k = sqrt(2.0) / 2.0;
    auto n = normal_at(s, point(0.0, k, -k));
    EXPECT_TRUE(almost_equal(n, vector(0.0, 0.97014, -0.24245), 1e-4));
}

// A sphere has a default material
TEST(TestSphere, sphere_has_default_material) {
    auto s = sphere(1);
    auto m = s.material();
    EXPECT_EQ(m, material());
}

// A sphere may be assigned a material
TEST(TestSphere, sphere_may_be_assigned_material) {
    auto s = sphere(1);
    auto m = material();
    m.set_ambient(1.0);
    s.set_material(m);
    EXPECT_EQ(s.material(), m);
}
