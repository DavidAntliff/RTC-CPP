// Chapter 10 - Patterns - Perlin Noise

#include <gtest/gtest.h>

#include <ray_tracer_challenge/perlin_noise.h>
#include <ray_tracer_challenge/canvas.h>

using namespace rtc;

TEST(TestPerlin, test_something) {
    auto pn = perlin_noise();

//    auto image {canvas(512, 512)};
//    auto scale = 4.0;
//    for (int x = 0; x < image.width(); ++x) {
//        for (int y = 0; y < image.height(); ++y) {
//            double dx = scale * static_cast<double>(x) / image.width(); //* 0.1;
//            double dy = scale * static_cast<double>(y) / image.height(); //* 0.1;
//            auto const v = pn.octave_perlin(dx, dy, 0.0, 10, 0.9);
//            auto pixel = color(v, v, v);
//            write_pixel(image, x, y, pixel);
//        }
//    }
//    auto ppm = ppm_from_canvas(image);
//    write_to_file("test_perlin_noise.ppm", ppm);

    EXPECT_EQ(0.5, pn.perlin(0.0, 0.0, 0.0));
}


