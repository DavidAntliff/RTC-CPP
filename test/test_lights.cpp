// Chapter 6: Lights and Shading

#include <gtest/gtest.h>

#include <ray_tracer_challenge/lights.h>
#include <ray_tracer_challenge/tuples.h>
#include <ray_tracer_challenge/color.h>

using namespace rtc;

// A point light has a position and intensity
TEST(TestLights, point_light_has_position_and_intensity) {
    auto intensity = color(1.0, 1.0, 1.0);
    auto position = point(0.0, 0.0, 0.0);
    auto light = point_light(position, intensity);
    EXPECT_EQ(light.position(), position);
    EXPECT_EQ(light.intensity(), intensity);
}
