#ifndef RTC_LIB_MATRIX_BUILDER_H
#define RTC_LIB_MATRIX_BUILDER_H

// Inspiration: https://hackernoon.com/design-patterns-builder-pattern-in-modern-c-x1283uy3

namespace rtc {

template <typename Matrix>
class MatrixBuilder {
public:
    MatrixBuilder(Matrix m) : matrix(m) {}

    // Use explicit type to invoke implicit conversion to Matrix:
    operator Matrix() const { return std::move(matrix); }

    // Or use .end() suffix to have auto deduce the type as Matrix:
    auto end() { return std::move(matrix); }

    MatrixBuilder & then(Matrix m) { matrix = m * matrix; return *this; }


private:
    Matrix matrix;
};

}

#endif // RTC_LIB_MATRIX_BUILDER_H
