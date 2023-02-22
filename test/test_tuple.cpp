// Chapter 1: Tuples, Points and Vectors

#include <gtest/gtest.h>

#include <ray_tracer_challenge/tuple.h>

using namespace rtc;

// The default Tuple is all zero
TEST(TestTuple, default_tuple) {
    auto a = Tuple();
    EXPECT_EQ(a.x(), 0);
    EXPECT_EQ(a.y(), 0);
    EXPECT_EQ(a.z(), 0);
    EXPECT_EQ(a.w(), 0);
}

// Invalid access via .at() returns empty optional
TEST(TestTuple, invalid_access) {
    auto a = Tuple();
    EXPECT_EQ(a.at(-1), std::nullopt);
    EXPECT_EQ(a.at(4), std::nullopt);
}

// A tuple with w=1.0 is a point
TEST(TestTuple, tuple_is_a_point) {
    auto a = tuple<float>(4.3, -4.2, 3.1, 1.0);
    EXPECT_FLOAT_EQ(a.x(), 4.3);
    EXPECT_FLOAT_EQ(a.y(), -4.2);
    EXPECT_FLOAT_EQ(a.z(), 3.1);
    EXPECT_EQ(a.w(), 1.0);

    EXPECT_FLOAT_EQ(a(0), 4.3);
    EXPECT_FLOAT_EQ(a(1), -4.2);
    EXPECT_FLOAT_EQ(a(2), 3.1);
    EXPECT_EQ(a(3), 1.0);

    EXPECT_FLOAT_EQ(*a.at(0), 4.3);
    EXPECT_FLOAT_EQ(*a.at(1), -4.2);
    EXPECT_FLOAT_EQ(*a.at(2), 3.1);
    EXPECT_EQ(*a.at(3), 1.0);

    EXPECT_TRUE(a.is_point());
    EXPECT_FALSE(a.is_vector());
}

// A tuple with w=0 is a vector
TEST(TestTuple, tuple_is_a_vector) {
    auto a = tuple<float>(4.3, -4.2, 3.1, 0.0);
    EXPECT_FLOAT_EQ(a.x(), 4.3);
    EXPECT_FLOAT_EQ(a.y(), -4.2);
    EXPECT_FLOAT_EQ(a.z(), 3.1);
    EXPECT_EQ(a.w(), 0.0);
    EXPECT_FALSE(a.is_point());
    EXPECT_TRUE(a.is_vector());
}

// point() creates tuples with w=1
TEST(TestTuple, point_creates_tuple) {
    auto p = point(4, -4, 3);
    EXPECT_TRUE(p.is_point());
    EXPECT_EQ(p, Tuple(4, -4, 3, 1));
}

// vector() creates tuples with w=0
TEST(TestTuple, vector_creates_tuple) {
    auto v = vector(4, -4, 3);
    EXPECT_TRUE(v.is_vector());
    EXPECT_EQ(v, Tuple(4, -4, 3, 0));
}

// Adding two tuples
TEST(TestTuple, adding_two_tuples) {
    auto a1 = tuple(3, -2, 5, 1);
    auto a2 = tuple(-2, 3, 1, 0);
    EXPECT_EQ(a1 + a2, tuple(1, 1, 6, 1));
}

// Subtracting two points
TEST(TestTuple, subtracting_two_points) {
    auto p1 = point(3, 2, 1);
    auto p2 = point(5, 6, 7);
    EXPECT_EQ(p1 - p2, vector(-2, -4, -6));
}

// Subtracting a vector from a point
TEST(TestTuple, subtracting_vector_from_point) {
    auto p = point(3, 2, 1);
    auto v = vector(5, 6, 7);
    EXPECT_EQ(p - v, point(-2, -4, -6));
}

// Subtracting two vectors
TEST(TestTuple, subtracting_two_vectors) {
    auto v1 = vector(3, 2, 1);
    auto v2 = vector(5, 6, 7);
    EXPECT_EQ(v1 - v2, vector(-2, -4, -6));
}

// Subtracting a vector from the zero vector
TEST(TestTuple, subtracting_vector_from_zero_vector) {
    auto zero = vector(0, 0, 0);
    auto v = vector(1, -2, 3);
    EXPECT_EQ(zero - v, vector(-1, 2, -3));
}

// Negating a tuple
TEST(TestTuple, negating_a_tuple) {
    auto a = tuple(1, -2, 3, -4);
    EXPECT_EQ(-a, tuple(-1, 2, -3, 4));
}

// Multiplying a tuple by a scalar
TEST(TestTuple, multiplying_tuple_by_scalar) {
    auto a = tuple(1., -2., 3., -4.);
    EXPECT_EQ(a * 3.5, tuple(3.5, -7., 10.5, -14.));
}

TEST(TestTuple, multiplying_tuple_by_scalar_prefix) {
    auto a = tuple(1., -2., 3., -4.);
    EXPECT_EQ(3.5 * a, tuple(3.5, -7., 10.5, -14.));
}

// Multiplying a tuple by a fraction
TEST(TestTuple, multiplying_tuple_by_fraction) {
    auto a = tuple(1., -2., 3., -4.);
    EXPECT_EQ(a * 0.5, tuple(0.5, -1., 1.5, -2.));
}

TEST(TestTuple, multiplying_tuple_by_fraction_prefix) {
    auto a = tuple(1., -2., 3., -4.);
    EXPECT_EQ(0.5 * a, tuple(0.5, -1., 1.5, -2.));
}

// Dividing a tuple by a scalar
TEST(TestTuple, dividing_tuple_by_scalar) {
    auto a = tuple(1., -2., 3., -4.);
    EXPECT_EQ(a / 2.0, tuple(0.5, -1., 1.5, -2.));
}

// Computing the magnitude of vector(1, 0, 0)
TEST(TestTuple, compute_magnitude_vector_1_0_0) {
    auto v = vector(1, 0, 0);
    EXPECT_EQ(magnitude(v), 1);
}

// Computing the magnitude of vector(0, 1, 0)
TEST(TestTuple, compute_magnitude_vector_0_1_0) {
    auto v = vector(0, 1, 0);
    EXPECT_EQ(magnitude(v), 1);
}

// Computing the magnitude of vector(0, 0, 1)
TEST(TestTuple, compute_magnitude_vector_0_0_1) {
    auto v = vector(0, 0, 1);
    EXPECT_EQ(magnitude(v), 1);
}

// Computing the magnitude of vector(1, 2, 3)
TEST(TestTuple, compute_magnitude_vector_1_2_3) {
    auto v = vector<float>(1., 2., 3.);
    EXPECT_FLOAT_EQ(magnitude(v), std::sqrt(14));
}

// Computing the magnitude of vector(-1, -2, -3)
TEST(TestTuple, compute_magnitude_vector_n1_n2_n3) {
    auto v = vector(-1., -2., -3.);
    EXPECT_FLOAT_EQ(magnitude(v), std::sqrt(14));
}

// Normalizing vector(4, 0, 0) gives (1, 0, 0)
TEST(TestTuple, normalizing_vector_4_0_0) {
    auto v = vector(4., 0., 0.);
    EXPECT_EQ(normalize(v), vector(1., 0., 0.));
}

// Normalizing vector(1, 2, 3)
TEST(TestTuple, normalizing_vector_1_2_3) {
    auto v = vector(1., 2., 3.);
    const auto sqrt14 = std::sqrt(14);
    EXPECT_EQ(normalize(v), vector(1 / sqrt14, 2 / sqrt14, 3 / sqrt14));
}

// The magnitude of a normalized vector
TEST(TestTuple, magnitude_of_normalized_vector) {
    auto v = vector<float>(1., 2., 3.);
    auto norm = normalize(v);
    EXPECT_FLOAT_EQ(magnitude(norm), 1.0);
}

// The dot product of two tuples
TEST(TestTuple, dot_product_of_two_tuples) {
    auto a = vector<float>(1., 2., 3.);
    auto b = vector<float>(2., 3., 4.);
    EXPECT_DOUBLE_EQ(dot(a, b), 20.0);
}

// The cross product of two vectors
TEST(TestTuple, cross_product_of_two_vectors) {
    auto a = vector(1., 2., 3.);
    auto b = vector(2., 3., 4.);
    EXPECT_EQ(cross(a, b), vector(-1., 2., -1.));
    EXPECT_EQ(cross(b, a), vector(1., -2., 1.));
}
