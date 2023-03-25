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

using namespace rtc;

using namespace std::numbers;

int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    auto w = world();

    auto floor = plane();
    floor.set_material(material());
    floor.material().set_color(color(1.0, 0.9, 0.9));
    floor.material().set_specular(0.0);

    auto wall = plane();
    wall.set_transform(rotation_x(pi / 2.0)
                        .then(rotation_y(0.3))
                        .then(translation(0.0, 0.0, 7.0)));
    wall.set_material(material());
    wall.material().set_color(Color(1.0, 0.8, 0.8));
    wall.material().set_specular(0.0);

    auto middle = sphere(4);
    middle.set_transform(translation(-0.5, 1.0, 0.5));
    middle.material() = material();
    middle.material().set_color(color(0.1, 1.0, 0.5));
    middle.material().set_diffuse(0.7);
    middle.material().set_specular(0.3);

    auto right = sphere(5);
    right.set_transform(translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5));
    right.material() = material();
    right.material().set_color(color(0.5, 1.0, 0.1));
    right.material().set_diffuse(0.7);
    right.material().set_specular(0.3);

    auto left = sphere(6);
    left.set_transform(translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33));
    left.material() = material();
    left.material().set_color(color(1.0, 0.8, 0.1));
    left.material().set_diffuse(0.7);
    left.material().set_specular(0.3);

//    auto left_up = sphere(7);
//    left_up.set_transform(translation(-2.0, 1.8, -1.0) * scaling(0.33, 0.33, 0.33));
//    left_up.material() = material();
//    left_up.material().set_color(color(1.0, 0.0, 0.0));
//    left_up.material().set_diffuse(0.7);
//    left_up.material().set_specular(0.6);

    w.add_object(floor);
    w.add_object(wall);
    w.add_object(middle);
    w.add_object(right);
    w.add_object(left);
//    w.add_object(left_up);
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
