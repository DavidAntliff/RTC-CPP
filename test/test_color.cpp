// Chapter 2: Drawing on a Canvas

#include <gtest/gtest.h>

#include <ray_tracer_challenge/color.h>
//#include "support/tuples.h"

using namespace rtc;

// Colors are (red, green, blue) tuples
TEST(TestColor, colors_are_tuples) {
    auto c = color(-0.5, 0.4, 1.7);
    EXPECT_FLOAT_EQ(c.red(), -0.5);
    EXPECT_FLOAT_EQ(c.green(), 0.4);
    EXPECT_FLOAT_EQ(c.blue(), 1.7);
}

// Adding colors
TEST(TestColor, adding_colors) {
    auto c1 = color(0.9, 0.6, 0.75);
    auto c2 = color(0.7, 0.1, 0.25);
    EXPECT_EQ(c1 + c2, color(1.6, 0.7, 1.0));
}

// Subtracting colors
TEST(TestColor, subtracting_colors) {
    auto c1 = color(0.9, 0.6, 0.75);
    auto c2 = color(0.7, 0.1, 0.25);
    EXPECT_TRUE(almost_equal(c1 - c2, color(0.2, 0.5, 0.5)));
    //EXPECT_TRUE(rtc::test::AlmostEqual<Tuple<double>>(c1 - c2, color(0.2, 0.5, 0.5)));
}

// Multiplying a color by a scalar
TEST(TestColor, multiplying_color_by_scalar) {
    auto c = color(0.2, 0.3, 0.4);
    EXPECT_EQ(c * 2, color(0.4, 0.6, 0.8));
}

// Multiplying colors
TEST(TestColor, multiplying_colors) {
    auto c1 = color(1., 0.2, 0.4);
    auto c2 = color(0.9, 1, 0.1);
    EXPECT_TRUE(almost_equal(c1 * c2, color(0.9, 0.2, 0.04)));
    EXPECT_TRUE(almost_equal(hadamard(c1, c2), color(0.9, 0.2, 0.04)));
}
