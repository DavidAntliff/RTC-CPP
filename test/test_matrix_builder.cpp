#include <gtest/gtest.h>

#include <numbers>

#include <ray_tracer_challenge/matrix_builder.h>
#include <ray_tracer_challenge/transformations.h>
#include <ray_tracer_challenge/tuple.h>

using namespace rtc;
using namespace rtc::default_tuple;

TEST(TestMatrixBuilder, fluent_interface) {

    auto p = point(1.0, 0.0, 1.0);

    // Explicit type, no need for .end()
    Matrix<double, 4> M = identity4x4<double>()
            .then(rotation_x(std::numbers::pi / 2.0))
            .then(scaling(5.0, 5.0, 5.0))
            .then(translation(10.0, 5.0, 7.0));

    EXPECT_EQ(M * p, point(15.0, 0.0, 7.0));

    // auto type, needs .end()
    auto M2 = identity4x4<double>()
            .then(rotation_x(std::numbers::pi / 2.0))
            .then(scaling(3.0, 3.0, 3.0))
            .then(translation(10.0, 5.0, 7.0))
            .end();

    EXPECT_EQ(M2 * p, point(13.0, 2.0, 7.0));
}
