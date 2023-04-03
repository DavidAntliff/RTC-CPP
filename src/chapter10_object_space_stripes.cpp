#include <iostream>
#include <numbers>

#include <ray_tracer_challenge/matrices.h>
#include <ray_tracer_challenge/tuples.h>
#include <ray_tracer_challenge/canvas.h>
#include <ray_tracer_challenge/transformations.h>
#include <ray_tracer_challenge/spheres.h>
#include <ray_tracer_challenge/planes.h>
#include <ray_tracer_challenge/lights.h>
#include <ray_tracer_challenge/materials.h>
#include <ray_tracer_challenge/world.h>
#include <ray_tracer_challenge/camera.h>
#include <ray_tracer_challenge/patterns.h>

using namespace rtc;

using namespace std::numbers;

int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    auto w = world();

    auto floor = plane();
    auto floor_pattern = stripe_pattern(black, white);
    floor.material().set_pattern(floor_pattern);
    w.add_object(floor);

    // No rotation of object, scaling or rotation or translation of patterns
    // should result in each sphere being exactly half black, half white.

    auto middle = sphere(4);
    middle.set_transform(translation(-0.5, 1.0, 0.5));
    auto middle_pattern = stripe_pattern(black, white);
    middle.material().set_pattern(middle_pattern);
    w.add_object(middle);

    auto right = sphere(5);
    right.set_transform(translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5));
    auto right_pattern = stripe_pattern(black, white);
    right.material().set_pattern(right_pattern);
    w.add_object(right);

    auto left = sphere(6);
    left.set_transform(translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33));
    auto left_pattern = stripe_pattern(black, white);
    left.material().set_pattern(left_pattern);
    w.add_object(left);

    w.add_light(point_light(point(-10.0, 10.0, -10.0), color(1.0, 1.0, 1.0)));

    //auto cam = camera(100, 50, pi / 3.0);
    //auto cam = camera(400, 200, pi / 3.0);
    auto cam = camera(1024, 768, pi / 3.0);
    //auto cam = camera(2048, 1536, pi / 3.0);
    cam.set_transform(view_transform(point(0.0, 1.5, -5.0),
                                     point(0.0, 1.0, 0.0),
                                     vector(0.0, 1.0, 0.0)));

    auto canvas = render(cam, w);

    auto ppm = ppm_from_canvas(canvas);
    std::cout << ppm;

    return 0;
}
