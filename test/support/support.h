#ifndef TEST_SUPPORT_SUPPORT_H
#define TEST_SUPPORT_SUPPORT_H

#include <gtest/gtest.h>

template <typename T, typename U>
::testing::AssertionResult AlmostEqual(T const & a, U const & b, double epsilon=1e-5) {
    if (almost_equal(a, b, epsilon)) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure() << a << " not almost equal to " << b << "(epsilon " << epsilon << ")";
    }
}

#endif // TEST_SUPPORT_SUPPORT_H
