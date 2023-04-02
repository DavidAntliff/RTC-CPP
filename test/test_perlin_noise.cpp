// Chapter 10 - Patterns - Perlin Noise

#include <gtest/gtest.h>

#include <ray_tracer_challenge/perlin_noise.h>
#include <ray_tracer_challenge/canvas.h>

using namespace rtc;

TEST(TestPerlin, generate_octave_perlin_noise_image) {
    auto pn = perlin_noise();

    auto image {canvas(512, 512)};
    auto coordinate_scale = 4.0;
    auto value_scale = 2.4;  // use to keep min and max between [0.0, 1.0]
    double min_vv = 1.0;
    double max_vv = 0.0;
    for (int x = 0; x < image.width(); ++x) {
        for (int y = 0; y < image.height(); ++y) {
            double dx = coordinate_scale * static_cast<double>(x) / image.width();
            double dy = coordinate_scale * static_cast<double>(y) / image.height();
            auto const v = pn.octave_perlin(dx, dy, 0.0, 8, 0.6);
            auto const vv = (v - 0.5) * value_scale + 0.5;
            min_vv = std::min(min_vv, vv);
            max_vv = std::max(max_vv, vv);
            auto pixel = color(vv, vv, vv);
            write_pixel(image, x, y, pixel);
        }
    }
    auto ppm = ppm_from_canvas(image);
    write_to_file("test_perlin_noise.ppm", ppm);
    std::cout << "min " << min_vv << ", max " << max_vv << '\n';

    EXPECT_EQ(0.5, pn.perlin(0.0, 0.0, 0.0));
}


