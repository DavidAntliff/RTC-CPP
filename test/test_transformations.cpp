// Chapter 4: Transformations

#include <gtest/gtest.h>

#include <numbers>

#include <ray_tracer_challenge/tuple.h>
#include <ray_tracer_challenge/matrix.h>
#include <ray_tracer_challenge/transformations.h>

using namespace rtc;

// Multiplying by a translation matrix
TEST(TestTransformations, multiplying_by_translation_matrix) {
    auto transform = translation(5.0, -3.0, 2.0);
    auto p = point(-3.0, 4.0, 5.0);
    EXPECT_EQ(transform * p, point(2.0, 1.0, 7.0));
}

// Multiplying by the inverse of a translation matrix
TEST(TestTransformations, multiplying_by_inverse_translation_matrix) {
    auto transform = translation(5.0, -3.0, 2.0);
    auto inv = inverse(transform);
    auto p = point(-3.0, 4.0, 5.0);
    EXPECT_EQ(inv * p, point(-8.0, 7.0, 3.0));
}

// Translation does not affect vectors
TEST(TestTransformations, translation_does_not_affect_vectors) {
    auto transform = translation(5.0, -3.0, 2.0);
    auto v = vector(-3.0, 4.0, 5.0);
    EXPECT_EQ(transform * v, v);
}

// A scaling matrix applied to a point
TEST(TestTransformations, scaling_matrix_applied_to_point) {
    auto transform = scaling(2.0, 3.0, 4.0);
    auto p = point(-4.0, 6.0, 8.0);
    EXPECT_EQ(transform * p, point(-8.0, 18.0, 32.0));
}

// A scaling matrix applied to a vector
TEST(TestTransformations, scaling_matrix_applied_to_vector) {
    auto transform = scaling(2.0, 3.0, 4.0);
    auto v = vector(-4.0, 6.0, 8.0);
    EXPECT_EQ(transform * v, vector(-8.0, 18.0, 32.0));
}

// Multiplying by the inverse of a scaling matrix
TEST(TestTransformations, multiplying_by_inverse_of_scaling_matrix) {
    auto transform = scaling(2.0, 3.0, 4.0);
    auto inv = inverse(transform);
    auto v = vector(-4.0, 6.0, 8.0);
    EXPECT_EQ(inv * v, vector(-2.0, 2.0, 2.0));
}

// Reflection is scaling by a negative value
TEST(TestTransformations, reflection_is_scaling_by_negative_value) {
    auto transform = scaling(-1.0, 1.0, 1.0);
    auto p = point(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(-2.0, 3.0, 4.0));
}

// Rotating a point around the x axis
TEST(TestTransformations, rotating_point_around_x_axis) {
    auto p = point(0.0, 1.0, 0.0);
    auto half_quarter = rotation_x(std::numbers::pi / 4.0);
    auto full_quarter = rotation_x(std::numbers::pi / 2.0);
    EXPECT_TRUE(almost_equal(half_quarter * p, point(0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0)));
    EXPECT_TRUE(almost_equal(full_quarter * p, point(0.0, 0.0, 1.0)));
}

// The inverse of an x-rotation rotates in the opposite direction
TEST(TestTransformations, inverse_rotate_around_x_axis) {
    auto p = point(0.0, 1.0, 0.0);
    auto half_quarter = rotation_x(std::numbers::pi / 4.0);
    auto inv = inverse(half_quarter);
    EXPECT_TRUE(almost_equal(inv * p, point(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0)));
}

// Rotating a point around the y axis
TEST(TestTransformations, rotating_point_around_y_axis) {
    auto p = point(0.0, 0.0, 1.0);
    auto half_quarter = rotation_y(std::numbers::pi / 4.0);
    auto full_quarter = rotation_y(std::numbers::pi / 2.0);
    EXPECT_TRUE(almost_equal(half_quarter * p, point(sqrt(2.0) / 2.0, 0.0, sqrt(2.0) / 2.0)));
    EXPECT_TRUE(almost_equal(full_quarter * p, point(1.0, 0.0, 0.0)));
}

// Rotating a point around the z axis
TEST(TestTransformations, rotating_point_around_z_axis) {
    auto p = point(0.0, 1.0, 0.0);
    auto half_quarter = rotation_z(std::numbers::pi / 4.0);
    auto full_quarter = rotation_z(std::numbers::pi / 2.0);
    EXPECT_TRUE(almost_equal(half_quarter * p, point(-sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0)));
    EXPECT_TRUE(almost_equal(full_quarter * p, point(-1.0, 0.0, 0.0)));
}

// A shearing transformation moves x in proportion to y
TEST(TestTransformations, shearing_moves_x_in_proportion_to_y) {
    auto transform = shearing(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    auto p = point(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(5.0, 3.0, 4.0));
}

// A shearing transformation moves x in proportion to z
TEST(TestTransformations, shearing_moves_x_in_proportion_to_z) {
    auto transform = shearing(0.0, 1.0, 0.0, 0.0, 0.0, 0.0);
    auto p = point(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(6.0, 3.0, 4.0));
}

// A shearing transformation moves y in proportion to x
TEST(TestTransformations, shearing_moves_y_in_proportion_to_x) {
    auto transform = shearing(0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
    auto p = point(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(2.0, 5.0, 4.0));
}

// A shearing transformation moves y in proportion to z
TEST(TestTransformations, shearing_moves_y_in_proportion_to_z) {
    auto transform = shearing(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    auto p = point(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(2.0, 7.0, 4.0));
}

// A shearing transformation moves z in proportion to x
TEST(TestTransformations, shearing_moves_z_in_proportion_to_x) {
    auto transform = shearing(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    auto p = point(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(2.0, 3.0, 6.0));
}

// A shearing transformation moves z in proportion to y
TEST(TestTransformations, shearing_moves_z_in_proportion_to_y) {
    auto transform = shearing(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    auto p = point(2.0, 3.0, 4.0);
    EXPECT_EQ(transform * p, point(2.0, 3.0, 7.0));
}

// Individual transformations are applied in sequence
TEST(TestTransformations, individual_transformations_applied_in_sequence) {
    auto p = point(1.0, 0.0, 1.0);
    auto A = rotation_x(std::numbers::pi / 2.0);
    auto B = scaling(5.0, 5.0, 5.0);
    auto C = translation(10.0, 5.0, 7.0);

    auto p2 = A * p;
    EXPECT_TRUE(almost_equal(p2, point(1.0, -1.0, 0.0)));

    auto p3 = B * p2;
    EXPECT_TRUE(almost_equal(p3, point(5.0, -5.0, 0.0)));

    auto p4 = C * p3;
    EXPECT_TRUE(almost_equal(p4, point(15.0, 0.0, 7.0)));
}

// Chained transformations must be applied in reverse order
TEST(TestTransformations, chained_transformations_applied_in_reverse_order) {
    auto p = point(1.0, 0.0, 1.0);
    auto A = rotation_x(std::numbers::pi / 2.0);
    auto B = scaling(5.0, 5.0, 5.0);
    auto C = translation(10.0, 5.0, 7.0);

    auto T = C * B * A;
    EXPECT_TRUE(almost_equal(T * p, point(15.0, 0.0, 7.0)));
}

// Fluent API
TEST(TestTransformations, fluent_api) {
    auto p = point(1.0, 0.0, 1.0);

    auto M = identity4x4<double>()
            .then(rotation_x(std::numbers::pi / 2.0))
            .then(scaling(5.0, 5.0, 5.0))
            .then(translation(10.0, 5.0, 7.0));

    EXPECT_EQ(M * p, point(15.0, 0.0, 7.0));

    auto M2 = identity4x4<double>()
            .then(rotation_x(std::numbers::pi / 2.0))
            .then(scaling(3.0, 3.0, 3.0))
            .then(translation(10.0, 5.0, 7.0));

    EXPECT_EQ(M2 * p, point(13.0, 2.0, 7.0));
}
