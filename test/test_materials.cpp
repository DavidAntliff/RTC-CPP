// Chapter 6: Lights and Shading

#include <gtest/gtest.h>

#include <ray_tracer_challenge/materials.h>
#include <ray_tracer_challenge/tuples.h>
#include <ray_tracer_challenge/lights.h>

using namespace rtc;

// The default material
TEST(TestMaterials, default_material) {
    auto m = material();
    EXPECT_EQ(m.color(), color(1.0, 1.0, 1.0));
    EXPECT_EQ(m.ambient(), 0.1);
    EXPECT_EQ(m.diffuse(), 0.9);
    EXPECT_EQ(m.specular(), 0.9);
    EXPECT_EQ(m.shininess(), 200.0);
}

class TestMaterialsFixture : public testing::Test {
protected:
    Material<double> m {material()};
    Point<double> position {point(0.0, 0.0, 0.0)};
};

// Lighting with the eye between the light and the surface
TEST_F(TestMaterialsFixture, lighting_with_eye_between_light_and_surface) {
    auto eyev = vector(0.0, 0.0, -1.0);
    auto normalv = vector(0.0, 0.0, -1.0);
    auto light = point_light(point(0.0, 0.0, -10.0), color(1.0, 1.0, 1.0));
    auto result = lighting(m, light, position, eyev, normalv);

    // intensity = full ambient + full diffuse + full specular
    EXPECT_EQ(result, color(1.9, 1.9, 1.9));
}

// Lighting with the eye between light and surface, eye offset 45 degrees
TEST_F(TestMaterialsFixture, lighting_with_eye_between_light_and_surface_eye_offset_45_degrees) {
    auto k = sqrt(2.0) / 2.0;
    auto eyev = vector(0.0, k, -k);
    auto normalv = vector(0.0, 0.0, -1.0);
    auto light = point_light(point(0.0, 0.0, -10.0), color(1.0, 1.0, 1.0));
    auto result = lighting(m, light, position, eyev, normalv);

    // intensity = full ambient + full diffuse + zero specular
    EXPECT_EQ(result, color(1.0, 1.0, 1.0));
}

// Lighting with eye opposite surface, light offset 45 degrees
TEST_F(TestMaterialsFixture, lighting_with_eye_opposite_surface_light_offset_45_degrees) {
    auto eyev = vector(0.0, 0.0, -1.0);
    auto normalv = vector(0.0, 0.0, -1.0);
    auto light = point_light(point(0.0, 10.0, -10.0), color(1.0, 1.0, 1.0));
    auto result = lighting(m, light, position, eyev, normalv);

    // intensity = full ambient + partial diffuse + zero specular
    EXPECT_TRUE(almost_equal(result, color(0.7364, 0.7364, 0.7364)));
}

// Lighting with eye in the path of the reflection vector
TEST_F(TestMaterialsFixture, lighting_with_eye_in_path_of_reflection_vector) {
    auto k = sqrt(2.0) / 2.0;
    auto eyev = vector(0.0, -k, -k);
    auto normalv = vector(0.0, 0.0, -1.0);
    auto light = point_light(point(0.0, 10.0, -10.0), color(1.0, 1.0, 1.0));
    auto result = lighting(m, light, position, eyev, normalv);

    // intensity = full ambient + partial diffuse + full specular
    EXPECT_TRUE(almost_equal(result, color(1.6364, 1.6364, 1.6364)));
}

// Lighting with the light behind the surface
TEST_F(TestMaterialsFixture, lighting_with_light_behind_surface) {
    auto eyev = vector(0.0, 0.0, -1.0);
    auto normalv = vector(0.0, 0.0, -1.0);
    auto light = point_light(point(0.0, 0.0, 10.0), color(1.0, 1.0, 1.0));
    auto result = lighting(m, light, position, eyev, normalv);

    // intensity = full ambient + zero diffuse + zero specular
    EXPECT_EQ(result, color(0.1, 0.1, 0.1));
}

