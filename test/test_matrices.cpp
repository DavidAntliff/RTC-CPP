// Chapter 3: Matrices

#include <gtest/gtest.h>

#include <ray_tracer_challenge/matrices.h>
#include <ray_tracer_challenge/tuples.h>

using namespace rtc;

TEST(TestMatrices, default_matrix_is_zero) {
    Matrix<4> A;
    for (auto r = 0; r < A.dim(); ++r) {
        for (auto c = 0; c < A.dim(); ++c) {
            EXPECT_EQ(A(r, c), 0.0);
            EXPECT_EQ(*A.at(r, c), 0.0);
        }
    }
}

TEST(TestMatrices, matrix_from_vector) {
    std::vector<double> el = {
        0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
    };
    Matrix<3> A {el};
    EXPECT_EQ(A, matrix3x3({
        { 0.0,  1.0,   2.0},
        { 3.0,  4.0,   5.0},
        { 6.0,  7.0,   8.0},
    }));
}

TEST(TestMatrices, invalid_access) {
    auto M = Matrix<4> {};
    EXPECT_EQ(M.at(0, -1), std::nullopt);
    EXPECT_EQ(M.at(-1, 0), std::nullopt);
    EXPECT_EQ(M.at(-1, -1), std::nullopt);
    EXPECT_EQ(M.at(4, 0), std::nullopt);
    EXPECT_EQ(M.at(0, 4), std::nullopt);
    EXPECT_EQ(M.at(4, 4), std::nullopt);
}

// Constructing and inspecting a 4x4 matrix
TEST(TestMatrices, constructing_and_inspecting_4x4) {
    Matrix<4> M44 {
        { 1.0,  2.0,  3.0,  4.0},
        { 5.5,  6.5,  7.5,  8.5},
        { 9.0, 10.0, 11.0, 12.0},
        {13.5, 14.5, 15.5, 16.5},
    };

    auto M = matrix4x4({
        { 1.0,  2.0,  3.0,  4.0},
        { 5.5,  6.5,  7.5,  8.5},
        { 9.0, 10.0, 11.0, 12.0},
        {13.5, 14.5, 15.5, 16.5},
    });

    EXPECT_EQ(M(0, 0), 1.0);
    EXPECT_EQ(M(0, 3), 4.0);
    EXPECT_EQ(M(1, 0), 5.5);
    EXPECT_EQ(M(1, 2), 7.5);
    EXPECT_EQ(M(2, 2), 11.0);
    EXPECT_EQ(M(3, 0), 13.5);
    EXPECT_EQ(M(3, 2), 15.5);
}

// Constructing and inspecting a 2x2 matrix
TEST(TestMatrices, constructing_and_inspecting_2x2) {
    Matrix<2> M22 {
        { -3.0,  5.0},
        {  1.0, -2.0},
    };

    auto M = matrix2x2({
        { -3.0,  5.0},
        {  1.0, -2.0},
    });

    EXPECT_EQ(M(0, 0), -3.0);
    EXPECT_EQ(M(0, 1), 5.0);
    EXPECT_EQ(M(1, 0), 1.0);
    EXPECT_EQ(M(1, 1), -2.0);
}

// Constructing and inspecting a 3x3 matrix
TEST(TestMatrices, constructing_and_inspecting_3x3) {
    Matrix<3> M33 {
        { -3.0,  5.0,  0.0},
        {  1.0, -2.0, -7.0},
        {  0.0,  1.0,  1.0},
    };

    auto M = matrix3x3({
        { -3.0,  5.0,  0.0},
        {  1.0, -2.0, -7.0},
        {  0.0,  1.0,  1.0},
    });

    EXPECT_EQ(M(0, 0), -3.0);
    EXPECT_EQ(M(1, 1), -2.0);
    EXPECT_EQ(M(2, 2), 1.0);
}

// Matrix equality with identical matrices
TEST(TestMatrices, equality_with_identical_matrices) {
    auto A = matrix4x4({
        { 1.0,  2.0,  3.0,  4.0},
        { 5.0,  6.0,  7.0,  8.0},
        { 9.0,  8.0,  7.0,  6.0},
        { 5.0,  4.0,  3.0,  2.0},
    });
    auto B = matrix4x4({
        { 1.0,  2.0,  3.0,  4.0},
        { 5.0,  6.0,  7.0,  8.0},
        { 9.0,  8.0,  7.0,  6.0},
        { 5.0,  4.0,  3.0,  2.0},
    });
    EXPECT_TRUE(A == B);
}

// Matrix equality with different matrices
TEST(TestMatrices, equality_with_different_matrices) {
    auto A = matrix4x4({
        { 1.0,  2.0,  3.0,  4.0},
        { 5.0,  6.0,  7.0,  8.0},
        { 9.0,  8.0,  7.0,  6.0},
        { 5.0,  4.0,  3.0,  2.0},
    });
    auto B = matrix4x4({
        { 1.0,  2.0,  3.0,  4.0},
        { 6.0,  7.0,  8.0,  9.0},
        { 9.0,  8.0,  7.0,  6.0},
        { 5.0,  4.0,  3.0,  2.0},
    });
    EXPECT_FALSE(A == B);
}

// Multiplying two matrices
TEST(TestMatrices, multiplying_two_matrices) {
    auto A = matrix4x4({
        { 1.0,  2.0,  3.0,  4.0},
        { 5.0,  6.0,  7.0,  8.0},
        { 9.0,  8.0,  7.0,  6.0},
        { 5.0,  4.0,  3.0,  2.0},
    });
    auto B = matrix4x4({
        {-2.0,  1.0,  2.0,  3.0},
        { 3.0,  2.0,  1.0, -1.0},
        { 4.0,  3.0,  6.0,  5.0},
        { 1.0,  2.0,  7.0,  8.0},
    });
    EXPECT_EQ(A * B, matrix4x4({
        {20.0, 22.0,  50.0,  48.0},
        {44.0, 54.0, 114.0, 108.0},
        {40.0, 58.0, 110.0, 102.0},
        {16.0, 26.0,  46.0,  42.0},
   }));
}

// A matrix multiplied by a tuple
TEST(TestMatrices, matrix_multipied_by_tuple) {
    auto A = matrix4x4({
        { 1.0,  2.0,  3.0,  4.0},
        { 2.0,  4.0,  4.0,  2.0},
        { 8.0,  6.0,  4.0,  1.0},
        { 0.0,  0.0,  0.0,  1.0},
    });
    auto b = tuple(1.0, 2.0, 3.0, 1.0);
    EXPECT_EQ(A * b, tuple(18.0, 24.0, 33.0, 1.0));
}

// Multiplying a matrix by the identity matrix
TEST(TestMatrices, matrix_multiplied_by_identity) {
    auto A = matrix4x4({
        { 0.0,  1.0,  2.0,  4.0},
        { 1.0,  2.0,  4.0,  8.0},
        { 2.0,  4.0,  8.0, 16.0},
        { 4.0,  8.0, 16.0, 32.0},
    });
    auto I = identity4x4();
    EXPECT_EQ(A * I, A);
    EXPECT_EQ(I * A, A);
}

// Multiplying the identity matrix by a tuple
TEST(TestMatrices, identity_multiplied_by_tuple) {
    auto a = tuple(1.0, 2.0, 3.0, 4.0);
    auto I = identity4x4();
    EXPECT_EQ(I * a, a);
}

// Transposing a matrix
TEST(TestMatrices, transposing_a_matrix) {
    auto A = matrix4x4({
        { 0.0,  9.0,  3.0,  0.0},
        { 9.0,  8.0,  0.0,  8.0},
        { 1.0,  8.0,  5.0,  3.0},
        { 0.0,  0.0,  5.0,  8.0},
    });

    EXPECT_EQ(transpose(A), matrix4x4({
        { 0.0,  9.0,   1.0,   0.0},
        { 9.0,  8.0,   8.0,   0.0},
        { 3.0,  0.0,   5.0,   5.0},
        { 0.0,  8.0,   3.0,   8.0},
    }));
}

// Transposing the identity matrix
TEST(TestMatrices, transposing_the_identity) {
    auto I = identity4x4();
    EXPECT_EQ(transpose(I), I);
}

// Calculating the determinant of a 2x2 matrix
TEST(TestMatrices, calculate_determinant_2x2) {
    auto A = matrix2x2({
        { 1.0, 5.0},
        {-3.0, 2.0},
    });
    EXPECT_EQ(determinant(A), 17.0);
}

// A submatrix of a 3x3 matrix is a 2x2 matrix
TEST(TestMatrices, submatrix_of_3x3) {
    auto A = matrix3x3({
        {  1.0,  5.0,  0.0},
        { -3.0,  2.0,  7.0},
        {  0.0,  6.0, -3.0},
    });

    EXPECT_EQ(submatrix(A, 0, 2), matrix2x2({
        {-3.0, 2.0},
        { 0.0, 6.0},
    }));
}

// A submatrix of a 4x4 matrix is a 3x3 matrix
TEST(TestMatrices, submatrix_of_4x4) {
    auto A = matrix4x4({
        {-6.0,  1.0,  1.0,  6.0},
        {-8.0,  5.0,  8.0,  6.0},
        {-1.0,  0.0,  8.0,  2.0},
        {-7.0,  1.0, -1.0,  1.0},
    });

    EXPECT_EQ(submatrix(A, 2, 1), matrix3x3({
        {-6.0, 1.0, 6.0},
        {-8.0, 8.0, 6.0},
        {-7.0,-1.0, 1.0},
    }));
}

// Calculating a minor of a 3x3 matrix
TEST(TestMatrices, calculate_minor_of_3x3) {
    auto A = matrix3x3({
        {  3.0,  5.0,  0.0},
        {  2.0, -1.0, -7.0},
        {  6.0, -1.0,  5.0},
    });
    auto B = submatrix(A, 1, 0);
    EXPECT_EQ(determinant(B), 25.0);
    EXPECT_EQ(minor(A, 1, 0), 25.0);
}

// Calculating a cofactor of a 3x3 matrix
TEST(TestMatrices, calculate_cofactor_of_3x3) {
    auto A = matrix3x3({
        {  3.0,  5.0,  0.0},
        {  2.0, -1.0, -7.0},
        {  6.0, -1.0,  5.0},
    });
    EXPECT_EQ(minor(A, 0, 0), -12.0);
    EXPECT_EQ(cofactor(A, 0, 0), -12.0);
    EXPECT_EQ(minor(A, 1, 0), 25.0);
    EXPECT_EQ(cofactor(A, 1, 0), -25.0);
}

// Calculating the determinant of a 3x3 matrix
TEST(TestMatrices, calculate_determinant_of_3x3) {
    auto A = matrix3x3({
        {  1.0,  2.0,  6.0},
        { -5.0,  8.0, -4.0},
        {  2.0,  6.0,  4.0},
    });
    EXPECT_EQ(cofactor(A, 0, 0), 56.0);
    EXPECT_EQ(cofactor(A, 0, 1), 12.0);
    EXPECT_EQ(cofactor(A, 0, 2), -46.0);
    EXPECT_EQ(determinant(A), -196.0);
}

// Calculating the determinant of a 4x4 matrix
TEST(TestMatrices, calculate_determinant_of_4x4) {
    auto A = matrix4x4({
        {-2.0, -8.0,  3.0,  5.0},
        {-3.0,  1.0,  7.0,  3.0},
        { 1.0,  2.0, -9.0,  6.0},
        {-6.0,  7.0,  7.0, -9.0},
    });
    EXPECT_EQ(cofactor(A, 0, 0), 690.0);
    EXPECT_EQ(cofactor(A, 0, 1), 447.0);
    EXPECT_EQ(cofactor(A, 0, 2), 210.0);
    EXPECT_EQ(cofactor(A, 0, 3), 51.0);
    EXPECT_EQ(determinant(A), -4071.0);
}

// Testing an invertible matrix for invertibility
TEST(TestMatrices, invertible_matrix_is_invertible) {
    auto A = matrix4x4({
        { 6.0,  4.0,  4.0,  4.0},
        { 5.0,  5.0,  7.0,  6.0},
        { 4.0, -9.0,  3.0, -7.0},
        { 9.0,  1.0,  7.0, -6.0},
    });
    EXPECT_EQ(determinant(A), -2120.0);
    EXPECT_TRUE(is_invertible(A));
}

// Testing a noninvertible matrix for invertibility
TEST(TestMatrices, noninvertible_matrix_is_not_invertible) {
    auto A = matrix4x4({
        {-4.0,  2.0, -2.0, -3.0},
        { 9.0,  6.0,  2.0,  6.0},
        { 0.0, -5.0,  1.0, -5.0},
        { 0.0,  0.0,  0.0,  0.0},
    });
    EXPECT_EQ(determinant(A), 0.0);
    EXPECT_FALSE(is_invertible(A));
}

// Calculating the inverse of a matrix
TEST(TestMatrices, calculate_inverse_of_matrix) {
    auto A = matrix4x4({
        {-5.0,  2.0,  6.0, -8.0},
        { 1.0, -5.0,  1.0,  8.0},
        { 7.0,  7.0, -6.0, -7.0},
        { 1.0, -3.0,  7.0,  4.0},
    });
    auto B = inverse(A);
    EXPECT_EQ(determinant(A), 532.0);
    EXPECT_EQ(cofactor(A, 2, 3), -160.0);
    EXPECT_EQ(B(3, 2), -160.0 / 532.0);
    EXPECT_EQ(cofactor(A, 3, 2), 105.0);
    EXPECT_EQ(B(2, 3), 105.0 / 532.0);
    EXPECT_TRUE(almost_equal(B, matrix4x4({
        { 0.21805,   0.45113,   0.24060,  -0.04511},
        {-0.80827,  -1.45677,  -0.44361,   0.52068},
        {-0.07895,  -0.22368,  -0.05263,   0.19737},
        {-0.52256,  -0.81391,  -0.30075,   0.30639},
    })));
}

// Calculating the inverse of another matrix
TEST(TestMatrices, calculate_inverse_of_another_matrix) {
    auto A = matrix4x4({
        { 8.0, -5.0,  9.0,  2.0},
        { 7.0,  5.0,  6.0,  1.0},
        {-6.0,  0.0,  9.0,  6.0},
        {-3.0,  0.0, -9.0, -4.0},
    });
    EXPECT_TRUE(almost_equal(inverse(A), matrix4x4({
        {-0.15385,  -0.15385,  -0.28205,  -0.53846},
        {-0.07692,   0.12308,   0.02564,   0.03077},
        { 0.35897,   0.35897,   0.43590,   0.92308},
        {-0.69231,  -0.69231,  -0.76923,  -1.92308},
    })));
}

// Calculating the inverse of a third matrix
TEST(TestMatrices, calculate_inverse_of_a_third_matrix) {
    auto A = matrix4x4({
        { 9.0,  3.0,  0.0,  9.0},
        {-5.0, -2.0, -6.0, -3.0},
        {-4.0,  9.0,  6.0,  4.0},
        {-7.0,  6.0,  6.0,  2.0},
    });
    EXPECT_TRUE(almost_equal(inverse(A), matrix4x4({
        {-0.04074, -0.07778,  0.14444, -0.22222},
        {-0.07778,  0.03333,  0.36667, -0.33333},
        {-0.02901, -0.14630, -0.10926,  0.12963},
        { 0.17778,  0.06667, -0.26667,  0.33333},
    })));
}

// Multiplying a product by its inverse}
TEST(TestMatrices, multiply_product_by_inverse) {
    auto A = matrix4x4({
        { 3.0,  -9.0,   7.0,   3.0},
        { 3.0,  -8.0,   2.0,  -9.0},
        {-4.0,   4.0,   4.0,   1.0},
        {-6.0,   5.0,  -1.0,   1.0},
    });
    auto B = matrix4x4({
        {8.0,   2.0,  2.0,  2.0},
        {3.0,  -1.0,  7.0,  0.0},
        {7.0,   0.0,  5.0,  4.0},
        {6.0,  -2.0,  0.0,  5.0},
    });
    auto C = A * B;
    EXPECT_TRUE(almost_equal(C * inverse(B), A));
}
