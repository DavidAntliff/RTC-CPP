// Chapter 2: Drawing On a Canvas

#include <gtest/gtest.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <ray_tracer_challenge/canvas.h>
#include <ray_tracer_challenge/color.h>

using namespace rtc;

// Creating a canvas
TEST(TestCanvas, creating_a_canvas) {
    auto c = canvas(10, 20);
    EXPECT_EQ(c.width(), 10);
    EXPECT_EQ(c.height(), 20);
    for (int x = 0; x < c.width(); ++x) {
        for (int y = 0; y < c.height(); ++y) {
            EXPECT_EQ(*pixel_at(c, x, y), Color(0., 0., 0.));
        }
    }
}

// Writing pixels to a canvas
TEST(TestCanvas, writing_pixels_to_a_canvas) {
    auto c = canvas(10, 20);
    auto red = color(1., 0., 0.);
    write_pixel(c, 2, 3, red);
    EXPECT_EQ(*pixel_at(c, 2, 3), red);
}

// Constructing the PPM header
TEST(TestCanvas, construct_ppm_header) {
    auto c = canvas(5, 3);
    auto ppm = ppm_from_canvas(c);
    EXPECT_TRUE(ppm.starts_with("P3\n5 3\n255\n"));
}

// Constructing the PPM pixel data
TEST(TestCanvas, construct_ppm_pixel_data) {
    auto c = canvas(5, 3);
    auto c1 = color(1.5, 0.0, 0.0);
    auto c2 = color(0.0, 0.5, 0.0);
    auto c3 = color(-0.5, 0.0, 1.0);
    write_pixel(c, 0, 0, c1);
    write_pixel(c, 2, 1, c2);
    write_pixel(c, 4, 2, c3);
    auto ppm = ppm_from_canvas(c);

    std::vector<std::string> lines;
    boost::split(lines, ppm, boost::is_any_of("\n"));

    EXPECT_EQ(lines[3], "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
    EXPECT_EQ(lines[4], "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");
    EXPECT_EQ(lines[5], "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
}

// Splitting long lines in PPM files
TEST(TestCanvas, splitting_long_lines_in_ppm_files) {
    auto c = canvas(10, 2);
    for (int y = 0; y < c.height(); ++y) {
        for (int x = 0; x < c.width(); ++x) {
            write_pixel(c, x, y, Color(1., 0.8, 0.6));
        }
    }
    auto ppm = ppm_from_canvas(c);

    std::vector<std::string> lines;
    boost::split(lines, ppm, boost::is_any_of("\n"));

    EXPECT_EQ(lines[3], "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    EXPECT_EQ(lines[4], "153 255 204 153 255 204 153 255 204 153 255 204 153");
    EXPECT_EQ(lines[5], "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    EXPECT_EQ(lines[6], "153 255 204 153 255 204 153 255 204 153 255 204 153");
}

// PPM files are terminated by a newline character
TEST(TestCanvas, ppm_terminated_by_newline) {
    auto c = canvas(5, 3);
    auto ppm = ppm_from_canvas(c);

    EXPECT_TRUE(ppm.ends_with('\n'));
}
