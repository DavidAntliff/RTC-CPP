// Chapter 10 - Patterns

#include <gtest/gtest.h>

#include <ray_tracer_challenge/patterns.h>

using namespace rtc;

// Creating a stripe pattern
TEST(TestPatterns, creating_a_stripe_pattern) {
    auto pattern = stripe_pattern(white, black);
    EXPECT_EQ(pattern.a(), white);
    EXPECT_EQ(pattern.b(), black);
}

// A stripe pattern is constant in y
TEST(TestPatterns, stripe_pattern_is_constant_in_y) {
    auto pattern = stripe_pattern(white, black);
    EXPECT_EQ(stripe_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(stripe_at(pattern, point(0.0, 1.0, 0.0)), white);
    EXPECT_EQ(stripe_at(pattern, point(0.0, 2.0, 0.0)), white);
}

// A stripe pattern is constant in z
TEST(TestPatterns, stripe_pattern_is_constant_in_z) {
    auto pattern = stripe_pattern(white, black);
    EXPECT_EQ(stripe_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(stripe_at(pattern, point(0.0, 0.0, 1.0)), white);
    EXPECT_EQ(stripe_at(pattern, point(0.0, 0.0, 2.0)), white);
}

// A stripe pattern alternates in x
TEST(TestPatterns, stripe_pattern_alternates_in_x) {
    auto pattern = stripe_pattern(white, black);
    EXPECT_EQ(stripe_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(stripe_at(pattern, point(0.9, 0.0, 0.0)), white);
    EXPECT_EQ(stripe_at(pattern, point(1.0, 0.0, 0.0)), black);
    EXPECT_EQ(stripe_at(pattern, point(-0.1, 0.0, 0.0)), black);
    EXPECT_EQ(stripe_at(pattern, point(-1.0, 0.0, 0.0)), black);
    EXPECT_EQ(stripe_at(pattern, point(-1.1, 0.0, 0.0)), white);
}

