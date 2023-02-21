#ifndef RTC_LIB_MATRIX_BUILDER_H
#define RTC_LIB_MATRIX_BUILDER_H

// Inspiration: https://hackernoon.com/design-patterns-builder-pattern-in-modern-c-x1283uy3

namespace rtc {

template <typename Matrix>
class MatrixBuilder {
public:
    MatrixBuilder() = default;
    MatrixBuilder(Matrix m) : matrix_(m) {}

    // Use explicit type to invoke implicit conversion to Matrix:
    operator Matrix() const { return std::move(matrix_); }

    // Or use .end() suffix to have auto deduce the type as Matrix:
    auto end() const { return std::move(matrix_); }

    MatrixBuilder & then(Matrix m) { matrix_ = m * matrix_; return *this; }

private:
    Matrix matrix_ {};
};

}

#endif // RTC_LIB_MATRIX_BUILDER_H
