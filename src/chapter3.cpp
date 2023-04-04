#include <iostream>

#include <ray_tracer_challenge/matrices.h>
#include <ray_tracer_challenge/tuples.h>

using namespace rtc;

int main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "Invert identity:" << inverse(identity4x4()) << '\n';

    auto a = matrix4x4({
        { 3.0,  -9.0,   7.0,   3.0},
        { 3.0,  -8.0,   2.0,  -9.0},
        {-4.0,   4.0,   4.0,   1.0},
        {-6.0,   5.0,  -1.0,   1.0},
    });
    std::cout << "Matrix A:" << a << '\n';

    auto a_inv = inverse(a);
    std::cout << "Multiply by own inverse:" << a * a_inv << '\n';
    // result is identity

    std::cout << "Inverse of transpose:" << inverse(transpose(a)) << '\n';
    std::cout << "Transpose of inverse:" << transpose(inverse(a)) << '\n';
    // they are the same

    auto t = tuple(4.0, 3.0, 2.0, 1.0);
    std::cout << "Tuple: " << t << '\n';

    auto i4 = identity4x4();
    i4.set(1, 1, 2.0);
    std::cout << "Modified I4 * tuple:" << i4 * t << '\n';
    // result is corresponding element in tuple is scaled

    return 0;
}
