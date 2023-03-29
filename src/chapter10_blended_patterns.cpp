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
    floor.material().set_specular(0.0);
    auto const scale {0.5};
    //auto floor_pattern_1 = stripe_pattern(white, green);
    auto floor_pattern_1 = stripe_pattern(white, black);
    //floor_pattern_1.set_transform(scaling(scale, scale, scale).then(rotation_y(pi / 4.0)));
    //auto floor_pattern_2 = stripe_pattern(white, green);
    auto floor_pattern_2 = stripe_pattern(white, black);
    //floor_pattern_2.set_transform(scaling(scale, scale, scale).then(rotation_y(-pi / 4.0)));
    floor_pattern_2.set_transform(scaling(scale, scale, scale).then(rotation_y(pi / 2.0)));
    auto floor_pattern = blended_pattern(floor_pattern_1, floor_pattern_2);
    floor.material().set_pattern(floor_pattern);
    w.add_object(floor);

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
