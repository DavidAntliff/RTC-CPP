#include <iostream>

#include <ray_tracer_challenge/matrix.h>
#include <ray_tracer_challenge/tuple.h>
#include <ray_tracer_challenge/canvas.h>
#include <ray_tracer_challenge/transformations.h>

using namespace rtc;
using namespace rtc::default_tuple;

template <typename Canvas>
void set_pixel(Canvas & c, double x, double y) {
    auto const x_off = c.width() / 2.0;
    auto const y_off = c.height() / 2.0;
    auto const scaling = 1.0;

    write_pixel(c, scaling * x + x_off, scaling * y + y_off, Color(1.0, 1.0, 1.0));
}

int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    auto c = canvas(900, 550);

    // 1 hour rotation (15 degrees)
    auto hour = rotation_z(2 * std::numbers::pi / 12);

    set_pixel(c, 0.0, 0.0);

    auto p = point(0.0, 120.0, 0.0);

    for (int i = 0; i < 12; ++i) {
        auto p2 = hour * p;
        set_pixel(c, p2.x(), p2.y());
        p = p2;
    }

    auto ppm = ppm_from_canvas(c);
    std::cout << ppm;

    return 0;
}
