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
    floor.set_material(material());
    floor.material().set_color(color(1.0, 0.9, 0.9));
    floor.material().set_ambient(0.2);
    floor.material().set_specular(0.0);
    auto floor_pattern_1 = perturbed_pattern(stripe_pattern(red, white), 2.0, 4, 0.9);
    auto floor_pattern_2 = perturbed_pattern(stripe_pattern(red, white), 2.0, 4, 0.9);
    auto scale {0.4};
    floor_pattern_1.set_transform(scaling(scale, scale, scale).then(rotation_y(pi / 4.0)));
    floor_pattern_2.set_transform(scaling(scale, scale, scale).then(rotation_y(-pi / 4.0)));
    auto floor_pattern = blended_pattern(floor_pattern_1, floor_pattern_2);
    set_pattern_transform(floor_pattern, rotation_y(-pi / 8.0));
    floor.material().set_pattern(floor_pattern);
    w.add_object(floor);

    auto middle = sphere(4);
    middle.set_transform(translation(-0.5, 1.0, 0.5));
    middle.material() = material();
    middle.material().set_color(color(0.1, 1.0, 0.5));
    middle.material().set_diffuse(0.9);
    middle.material().set_specular(0.7);
    auto middle_pattern = perturbed_pattern(
            stripe_pattern(color(13, 104, 53), color(15, 158,79)),
            2.0, 3, 0.8);
    set_pattern_transform(middle_pattern, scaling(0.25, 0.25, 0.25)
                              .then(rotation_z(-pi / 4.0))
                              .then(rotation_y(-pi / 4.0)));
    middle.material().set_pattern(middle_pattern);
    w.add_object(middle);

    auto right = sphere(5);
    right.set_transform(translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5));
    right.material() = material();
    right.material().set_color(color(0.5, 1.0, 0.1));
    right.material().set_diffuse(0.9);
    right.material().set_specular(0.3);
    auto right_pattern = perturbed_pattern(
            gradient_pattern(color(200, 40, 0), color(200, 180, 0)),
            0.8, 4, 0.9);
    set_pattern_transform(right_pattern, scaling(2.2, 2.2, 2.2)
                                .then(rotation_z(pi / 6.0))
                                .then(translation(2.0, 0.0, 0.0)));
    right.material().set_pattern(right_pattern);
    w.add_object(right);

    auto left = sphere(6);
    left.set_transform(translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33));
    left.material() = material();
    left.material().set_color(color(1.0, 0.8, 0.1));
    left.material().set_diffuse(0.9);
    left.material().set_specular(0.3);
    auto left_pattern = perturbed_pattern(
            ring_pattern(color(199, 240, 194), color(95, 191, 95)),
            1.5, 4, 0.9);
    set_pattern_transform(left_pattern, scaling(0.3, 0.3, 0.3)
                            .then(rotation_x(-pi / 3.0))
                            .then(rotation_y(-0.2)));
    left.material().set_pattern(left_pattern);
    w.add_object(left);

    w.add_light(point_light(point(-10.0, 10.0, -10.0), color(1.0, 1.0, 1.0)));

    //auto cam = camera(100, 50, pi / 3.0);
    //auto cam = camera(400, 200, pi / 3.0);
    //auto cam = camera(1024, 768, pi / 3.0);
    auto cam = camera(2048, 1536, pi / 3.0);
    cam.set_transform(view_transform(point(0.0, 1.5, -5.0),
                                     point(0.0, 1.0, 0.0),
                                     vector(0.0, 1.0, 0.0)));

    auto canvas = render(cam, w);

    auto ppm = ppm_from_canvas(canvas);
    std::cout << ppm;

    return 0;
}
