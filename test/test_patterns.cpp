// Chapter 10 - Patterns

#include <gtest/gtest.h>

#include <ray_tracer_challenge/spheres.h>
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
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 1.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 2.0, 0.0)), white);
}

// A stripe pattern is constant in z
TEST(TestPatterns, stripe_pattern_is_constant_in_z) {
    auto pattern = stripe_pattern(white, black);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 1.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 2.0)), white);
}

// A stripe pattern alternates in x
TEST(TestPatterns, stripe_pattern_alternates_in_x) {
    auto pattern = stripe_pattern(white, black);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.9, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(1.0, 0.0, 0.0)), black);
    EXPECT_EQ(pattern_at(pattern, point(-0.1, 0.0, 0.0)), black);
    EXPECT_EQ(pattern_at(pattern, point(-1.0, 0.0, 0.0)), black);
    EXPECT_EQ(pattern_at(pattern, point(-1.1, 0.0, 0.0)), white);
}

// Stripes with an object transformation
TEST(TestPatterns, stripes_with_an_object_transformation) {
    auto shape = sphere(1);
    set_transform(shape, scaling(2.0, 2.0, 2.0));
    auto pattern = stripe_pattern(white, black);
    auto c = pattern_at_shape(pattern, shape, point(1.5, 0.0, 0.0));
    EXPECT_EQ(c, white);
}

// Stripes with a pattern transformation
TEST(TestPatterns, stripes_with_a_pattern_transformation) {
    auto shape = sphere(1);
    auto pattern = stripe_pattern(white, black);
    set_pattern_transform(pattern, scaling(2.0, 2.0, 2.0));
    auto c = pattern_at_shape(pattern, shape, point(1.5, 0.0, 0.0));
    EXPECT_EQ(c, white);
}

// Stripes with an object and a pattern transformation
TEST(TestPatterns, stripes_with_an_object_and_a_pattern_transformation) {
    auto shape = sphere(1);
    set_transform(shape, scaling(2.0, 2.0, 2.0));
    auto pattern = stripe_pattern(white, black);
    set_pattern_transform(pattern, translation(0.5, 0.0, 0.0));
    auto c = pattern_at_shape(pattern, shape, point(2.5, 0.0, 0.0));
    EXPECT_EQ(c, white);
}

// Towards Generic Patterns:

class TestPattern : public Pattern<double> {
public:
    TestPattern() = default;

//    std::unique_ptr<Pattern<double>> clone() const override {
//        return std::make_unique<TestPattern>(*this);
//    }

    Color<double> pattern_at(Point<double> const & local_point) const override {
        return {local_point.x(), local_point.y(), local_point.z()};
    }

protected:
    // https://stackoverflow.com/a/43263477
    virtual TestPattern * clone_impl() const override { return new TestPattern
    (*this); };
};

TestPattern test_pattern() {
    return {};
}

// The default pattern transformation
TEST(TestPatterns, default_pattern_transformation) {
    auto pattern = test_pattern();
    EXPECT_EQ(pattern.transform(), identity4x4());
}

// Assigning a transformation
TEST(TestPatterns, assigning_a_transformation) {
    auto pattern = test_pattern();
    set_pattern_transform(pattern, translation(1.0, 2.0, 3.0));
    EXPECT_EQ(pattern.transform(), translation(1.0, 2.0, 3.0));
}

// A pattern with an object transformation
TEST(TestPatterns, pattern_with_an_object_transformation) {
    auto shape = sphere(1);
    set_transform(shape, scaling(2.0, 2.0, 2.0));
    auto pattern = test_pattern();
    auto c = pattern_at_shape(pattern, shape, point(2.0, 3.0, 4.0));
    EXPECT_EQ(c, color(1.0, 1.5, 2.0));
}

// A pattern with a pattern transformation
TEST(TestPatterns, pattern_with_a_pattern_transformation) {
    auto shape = sphere(1);
    auto pattern = test_pattern();
    set_pattern_transform(pattern, scaling(2.0, 2.0, 2.0));
    auto c = pattern_at_shape(pattern, shape, point(2.0, 3.0, 4.0));
    EXPECT_EQ(c, color(1.0, 1.5, 2.0));
}

// A pattern with an object and a pattern transformation
TEST(TestPatterns, pattern_with_an_object_and_a_pattern_transformation) {
    auto shape = sphere(1);
    set_transform(shape, scaling(2.0, 2.0, 2.0));
    auto pattern = test_pattern();
    set_pattern_transform(pattern, translation(0.5, 1.0, 1.5));
    auto c = pattern_at_shape(pattern, shape, point(2.5, 3.0, 3.5));
    EXPECT_EQ(c, color(0.75, 0.5, 0.25));
}

// Gradients

// A gradient linearly interpolates between colors
TEST(TestPatterns, gradient_linearly_interpolates_between_colors) {
    auto pattern = gradient_pattern(white, black);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.25, 0.0, 0.0)), color(0.75, 0.75, 0.75));
    EXPECT_EQ(pattern_at(pattern, point(0.5, 0.0, 0.0)), color(0.5, 0.5, 0.5));
    EXPECT_EQ(pattern_at(pattern, point(0.75, 0.0, 0.0)), color(0.25, 0.25, 0.25));
}

// Rings

// A ring should extend in both x and z
TEST(TestPatterns, ring_extends_in_both_x_and_z) {
    auto pattern = ring_pattern(white, black);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(1.0, 0.0, 0.0)), black);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 1.0)), black);
    EXPECT_EQ(pattern_at(pattern, point(0.708, 0.0, 0.708)), black);
}
