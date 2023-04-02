// Chapter 10 - Patterns

#include <gtest/gtest.h>
#include "support/support.h"

#include <ray_tracer_challenge/spheres.h>
#include <ray_tracer_challenge/canvas.h>
#include <ray_tracer_challenge/patterns.h>

using namespace rtc;

template <typename T>
static void dump_pattern(Pattern<T> const & pattern, int size=128, double scale=1.0) {
    std::string const name {::testing::UnitTest::GetInstance()->current_test_info()->name()};
    auto image {canvas(size, size)};
    for (int x = 0; x < image.width(); ++x) {
        for (int y = 0; y < image.height(); ++y) {
            double const dx {scale * static_cast<double>(x) / image.width()};
            double const dy {scale * static_cast<double>(y) / image.height()};
            auto const v {pattern_at(pattern, point(dx, 0.0, dy))};
            write_pixel(image, x, y, v);
            //std::cout << "(" << dx << ", " << dy << ") = " << pattern_at(pattern, point(dx, 0.0, dy)) << '\n';
        }
    }
    auto ppm {ppm_from_canvas(image)};
    write_to_file(name + ".ppm", ppm);
}

// Creating a stripe pattern
TEST(TestPatterns, creating_a_stripe_pattern) {
    auto pattern = stripe_pattern(white, black);
    EXPECT_EQ(pattern.a(), SolidPattern(white));
    EXPECT_EQ(pattern.b(), SolidPattern(black));
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
    virtual std::unique_ptr<Pattern<double>> clone_impl() const override {
        return std::make_unique<TestPattern>(*this);
    };
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

// 3D Checkers

// Checkers should repeat in x
TEST(TestPatterns, checkers_repeats_in_x) {
    auto pattern = checkers_pattern(white, black);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.99, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(1.01, 0.0, 0.0)), black);
}

// Checkers should repeat in y
TEST(TestPatterns, checkers_repeats_in_y) {
    auto pattern = checkers_pattern(white, black);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.99, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 1.01, 0.0)), black);
}

// Checkers should repeat in z
TEST(TestPatterns, checkers_repeats_in_z) {
    auto pattern = checkers_pattern(white, black);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.99)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 1.01)), black);
}

// Radial Gradient in x, y, z
TEST(TestPatterns, radial_gradient_linearly_interpolates_between_colors) {
    auto pattern = radial_gradient_pattern(white, black);
    EXPECT_EQ(pattern_at(pattern, point(0.0, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.25, 0.0, 0.0)), color(0.75, 0.75, 0.75));
    EXPECT_EQ(pattern_at(pattern, point(0.5, 0.0, 0.0)), color(0.5, 0.5, 0.5));
    EXPECT_EQ(pattern_at(pattern, point(0.75, 0.0, 0.0)), color(0.25, 0.25, 0.25));
    EXPECT_TRUE(almost_equal(pattern_at(pattern, point(1.0 - EPSILON, 0.0, 0.0)), black));

    auto side = [](double radius){ return sqrt(radius * radius / 2.0); };

    // radially in x, z
    auto x0 {side(0.25)};
    auto x1 {side(0.5)};
    auto x2 {side(0.75)};
    auto x3 {side(1.0 - EPSILON)};
    EXPECT_TRUE(AlmostEqual(pattern_at(pattern, point(x0, 0.0, x0)), color(0.75, 0.75, 0.75)));
    EXPECT_TRUE(AlmostEqual(pattern_at(pattern, point(x1, 0.0, x1)), color(0.5, 0.5, 0.5)));
    EXPECT_TRUE(AlmostEqual(pattern_at(pattern, point(x2, 0.0, x2)), color(0.25, 0.25, 0.25)));
    EXPECT_TRUE(AlmostEqual(pattern_at(pattern, point(x3, 0.0, x3)), black));
}

// Nested Patterns
TEST(TestPatterns, nested_patterns) {
    auto p0 = stripe_pattern(white, black);
    p0.set_transform(scaling(0.5, 0.5, 0.5));    // four stripes per unit
    auto p1 = stripe_pattern(green, red);
    p1.set_transform(scaling(0.5, 0.5, 0.5));    // four stripes per unit
    auto pattern = stripe_pattern(p0, p1);
    pattern.set_transform(scaling(0.5, 0.5, 0.5));  // two stripes per unit

    EXPECT_EQ(pattern_at(pattern, point(0.125, 0.0, 0.0)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.375, 0.0, 0.0)), black);
    EXPECT_EQ(pattern_at(pattern, point(0.625, 0.0, 0.0)), green);
    EXPECT_EQ(pattern_at(pattern, point(0.875, 0.0, 0.0)), red);
}

// Blended Patterns
TEST(TestPatterns, blended_patterns) {
    auto p0 = stripe_pattern(white, black);
    p0.set_transform(scaling(0.5, 0.5, 0.5));
    auto p1 = stripe_pattern(white, black);
    p1.set_transform(scaling(0.5, 0.5, 0.5).then(rotation_y(std::numbers::pi / 2.0)));
    auto pattern = blended_pattern(p0, p1);

    dump_pattern(pattern);

    auto const grey {color(0.5, 0.5, 0.5)};
    EXPECT_EQ(pattern_at(pattern, point(0.25, 0.0, 0.25)), grey);
    EXPECT_EQ(pattern_at(pattern, point(0.25, 0.0, 0.75)), white);
    EXPECT_EQ(pattern_at(pattern, point(0.75, 0.0, 0.25)), black);
    EXPECT_EQ(pattern_at(pattern, point(0.75, 0.0, 0.75)), grey);
}

// Perturbed Patterns
TEST(TestPatterns, perturbed_patterns) {
    auto p0 = stripe_pattern(white, black);
    auto pattern = perturbed_pattern(p0,
                                     1.9,  // scale
                                     4,    // num_octaves,
                                     0.9); // persistence
    dump_pattern(pattern, 1024, 4.0);
}
