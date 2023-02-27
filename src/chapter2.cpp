// Run:
//   $ cmake-build-debug/src/chapter2 > image.ppm
//   $ feh -Z --reload 1.0 image.ppm

#include <iostream>

#include <ray_tracer_challenge/canvas.h>
#include <ray_tracer_challenge/tuples.h>

using namespace rtc;

struct Projectile {
    Point position;
    Vector velocity;
};

struct Environment {
    Vector gravity;
    Vector wind;
};

Projectile tick(const Environment & environment,
                const Projectile & projectile) {
    return {
        projectile.position + projectile.velocity,
        projectile.velocity + environment.gravity + environment.wind
    };
}

int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    auto c = canvas(900, 550);

//    for (int y = 0; y < c.height(); ++y) {
//        for (int x = 0; x < c.width(); ++x) {
//            write_pixel(c, x, y, Color(1., 0.8, 0.6));
//        }
//    }

    Projectile projectile { point(0., 1., 0.), 11.25 * normalize(vector(1., 1.1, 0.)) };
    Environment environment { vector(0., -0.1, 0.), vector(-0.01, 0., 0.) };

    while (projectile.position.y() > 0) {
        write_pixel(c, projectile.position.x(), c.height() - projectile.position.y(), Color(1., 1., 1.));
        projectile = tick(environment, projectile);
    }

    auto ppm = ppm_from_canvas(c);
    std::cout << ppm;

    return 0;
}
