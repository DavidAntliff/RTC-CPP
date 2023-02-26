#include <iostream>
#include <numbers>

#include <ray_tracer_challenge/matrix.h>
#include <ray_tracer_challenge/tuple.h>
#include <ray_tracer_challenge/canvas.h>
#include <ray_tracer_challenge/transformations.h>
#include <ray_tracer_challenge/ray.h>

using namespace rtc;

using namespace std::numbers;

int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    // Sphere is at 0.0, 0.0, 0.0
    // Ray origin is z = -5.0
    // Wall is z = 10.0

    auto ray_origin = point(0.0, 0.0, -5.0);
    auto wall_z = 10.0;

    // Good size for wall is > 6 units for entire shadow
    auto wall_size = 7.0;

    auto canvas_pixels = 100;
    auto pixel_size = wall_size / canvas_pixels;
    auto half = wall_size / 2;

    auto c = canvas(canvas_pixels, canvas_pixels);
    auto red = color(1.0, 0.0, 0.0);
    auto shape = Sphere(1);

    // shrink along the y axis
    //shape.set_transform(scaling(1.0, 0.5, 1.0));

    // shrink along the x axis
    //shape.set_transform(scaling(0.5, 1.0, 1.0));

    // shrink it and rotate it
    //shape.set_transform(rotation_z(pi / 4) * scaling(0.5, 1.0, 1.0));

    // shrink it and skew it
    shape.set_transform(shearing(1.0, 0.0, 0.0, 0.0, 0.0, 0.0) * scaling(0.5, 1.0, 1.0));

    // for each row of pixels in the canvas
    for (auto y = 0; y < canvas_pixels; ++y) {
        // compute the world y coordinate (top = +half, bottom = -half)
        auto world_y = half - pixel_size * y;
        // for each pixel in the row
        for (auto x = 0; x < canvas_pixels; ++x) {
            // computer the world x coordinate (left = -half, right = +half)
            auto world_x = -half + pixel_size * x;
            // describe the point on the wall that the ray will target
            auto position = point(world_x, world_y, wall_z);

            auto r = ray(ray_origin, normalize(position - ray_origin));
            auto xs = intersect(shape, r);

            if (hit(xs)) {
                write_pixel(c, x, y, red);
            }
        }
    }

    auto ppm = ppm_from_canvas(c);
    std::cout << ppm;

    return 0;
}
