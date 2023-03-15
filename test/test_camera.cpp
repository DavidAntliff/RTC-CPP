// Chapter 7: Implementing a Camera

#include <gtest/gtest.h>

#include <numbers>

#include <ray_tracer_challenge/camera.h>
#include <ray_tracer_challenge/matrices.h>
#include <ray_tracer_challenge/world.h>

using namespace rtc;

constexpr auto pi = std::numbers::pi;

// Constructing a camera
TEST(TestCamera, constructing_a_camera) {
    auto hsize {160};
    auto vsize {120};
    auto field_of_view {pi / 2.0};
    auto c = camera(hsize, vsize, field_of_view);
    EXPECT_EQ(c.hsize(), 160);
    EXPECT_EQ(c.vsize(), 120);
    EXPECT_DOUBLE_EQ(c.field_of_view(), pi / 2.0);
    EXPECT_EQ(c.transform(), identity4x4());
}

// The pixel size for a horizontal canvas
TEST(TestCamera, pixel_size_for_horizontal_canvas) {
    auto c = camera(200, 125, pi / 2.0);
    EXPECT_DOUBLE_EQ(c.pixel_size(), 0.01);
}

// The pixel size for a vertical canvas
TEST(TestCamera, pixel_size_for_vertical_canvas) {
    auto c = camera(125, 200, pi / 2.0);
    EXPECT_DOUBLE_EQ(c.pixel_size(), 0.01);
}

// Constructing a ray through the center of the canvas
TEST(TestCamera, constructing_ray_through_center_of_canvas) {
    auto c = camera(201, 101, pi / 2.0);
    auto r = ray_for_pixel(c, 100, 50);
    EXPECT_EQ(r.origin(), point(0.0, 0.0, 0.0));
    EXPECT_TRUE(almost_equal(r.direction(), vector(0.0, 0.0, -1.0)));
}

// Constructing a ray through the corner of the canvas
TEST(TestCamera, constructing_ray_through_corner_of_canvas) {
    auto c = camera(201, 101, pi / 2.0);
    auto r = ray_for_pixel(c, 0, 0);
    EXPECT_EQ(r.origin(), point(0.0, 0.0, 0.0));
    EXPECT_TRUE(almost_equal(r.direction(), vector(0.66519, 0.33259, -0.66851)));
}

// Constructing a ray when the camera is transformed
TEST(TestCamera, constructing_ray_when_camera_is_transformed) {
    auto c = camera(201, 101, pi / 2.0);
    c.transform() = rotation_y(pi / 4.0) * translation(0.0, -2.0, 5.0);
    auto r = ray_for_pixel(c, 100, 50);
    EXPECT_EQ(r.origin(), point(0.0, 2.0, -5.0));
    auto const k = sqrt(2.0) / 2.0;
    EXPECT_TRUE(almost_equal(r.direction(), vector(k, 0.0, -k)));
}

// Rendering a world with a camera
TEST(TestCamera, rendering_world_with_camera) {
    auto w = default_world();
    auto c = camera(11, 11, pi / 2.0);
    auto from = point(0.0, 0.0, -5.0);
    auto to = point(0.0, 0.0, 0.0);
    auto up = vector(0.0, 1.0, 0.0);
    c.transform() = view_transform(from, to, up);
    auto image = render(c, w);
    EXPECT_TRUE(almost_equal(*pixel_at(image, 5, 5), color(0.38066, 0.47583, 0.2855)));
}
