#include <iostream>
#include <numbers>

#include <ray_tracer_challenge/matrices.h>
#include <ray_tracer_challenge/tuples.h>
#include <ray_tracer_challenge/canvas.h>
#include <ray_tracer_challenge/transformations.h>
#include <ray_tracer_challenge/rays.h>
#include <ray_tracer_challenge/spheres.h>
#include <ray_tracer_challenge/intersections.h>
#include <ray_tracer_challenge/lights.h>
#include <ray_tracer_challenge/materials.h>

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

    auto canvas_pixels = 200;
    auto pixel_size = wall_size / canvas_pixels;
    auto half = wall_size / 2;

    auto c = canvas(canvas_pixels, canvas_pixels);
    auto red = color(1.0, 0.0, 0.0);
    auto shape = Sphere(1);

    auto mat = material();
    //mat.set_color(color(1.0, 0.2, 1.0));
    mat.set_color(color(0.2, 1.0, 0.2));
    mat.set_specular(0.5);
    mat.set_diffuse(0.4);
    mat.set_shininess(10);
    shape.set_material(mat);

    // shrink along the y axis
    //shape.set_transform(scaling(1.0, 0.5, 1.0));

    // shrink along the x axis
    //shape.set_transform(scaling(0.5, 1.0, 1.0));

    // shrink it and rotate it
    shape.set_transform(rotation_z(-pi / 3) * scaling(0.5, 1.0, 1.0));

    // shrink it and skew it
    //shape.set_transform(shearing(1.0, 0.0, 0.0, 0.0, 0.0, 0.0) * scaling(0.5, 1.0, 1.0));

    //auto light_position = point(-10.0, 10.0, -10.0);
    auto light_position = point(10.0, -10.0, -10.0);
    //auto light_color = color(1.0, 1.0, 1.0);
    auto light_color = color(1.0, 1.0, 1.0);
    auto light = point_light(light_position, light_color);

    // for each row of pixels in the canvas
    for (auto y = 0; y < canvas_pixels; ++y) {
        if (y % 50 == 0) {
            std::cerr << "row " << y << '\n';
        }

        // compute the world y coordinate (top = +half, bottom = -half)
        auto world_y = half - pixel_size * y;
        // for each pixel in the row
        for (auto x = 0; x < canvas_pixels; ++x) {
            // computer the world x coordinate (left = -half, right = +half)
            auto world_x = -half + pixel_size * x;
            // describe the point on the wall that the ray will target
            auto pos = point(world_x, world_y, wall_z);

            auto r = ray(ray_origin, normalize(pos - ray_origin));
            auto xs = intersect(shape, r);

            auto h = hit(xs);
            if (h) {
                auto point = position(r, h->t());
                auto normal = normal_at(*h->object(), point);
                auto eye = -r.direction();

                auto color = lighting(h->object()->material(),
                                      light,
                                      point,
                                      eye,
                                      normal);

                write_pixel(c, x, y, color);
            }
        }
    }

    auto ppm = ppm_from_canvas(c);
    std::cout << ppm;

    return 0;
}
