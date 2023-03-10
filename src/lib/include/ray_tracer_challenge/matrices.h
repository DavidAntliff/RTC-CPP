#ifndef RTC_LIB_MATRICES_H
#define RTC_LIB_MATRICES_H

#include <array>
#include <initializer_list>
#include <ostream>
#include <vector>
#include <optional>

#include <boost/format.hpp>

#include "./math.h"

namespace rtc {

// Square Matrices only
template <typename T=fp_t, unsigned int N=0>
class Matrix {
public:
    Matrix() = default;

    Matrix(const std::initializer_list<std::initializer_list<T>> & elements)
    {
        assert(elements.size() == N);
        unsigned int counter = 0;
        for (auto row : elements) {
            assert(row.size() == N);
            std::copy(row.begin(), row.end(), elements_[counter++].begin());
        }
    }

    // Populate a matrix with a linear vector.
    // Element values are in rows.
    Matrix(const std::vector<T> & elements) {
        assert(elements.size() == N * N);
        typename std::vector<T>::const_iterator iter = elements.begin();
        for (auto r = 0U; r < N; ++r) {
            for (auto c = 0U; c < N; ++c) {
                elements_[r][c] = *iter;
                ++iter;
            }
        }
    }

    unsigned int dim() const { return N; }

    std::optional<T> at(unsigned int row, unsigned int column) const {
        if (row >= N)
            return std::nullopt;
        if (column >= N)
            return std::nullopt;
        return operator()(row, column);
    }

    // No bounds checking - undefined behaviour if row/column are out of bounds
    T operator()(unsigned int row, unsigned int column) const {
        return elements_[row][column];
    }

    void set(unsigned int row, unsigned int column, T value) {
        if (row >= N)
            return;
        if (column >= N)
            return;
        elements_[row][column] = value;
    }

    // Strict equality for floating point types
    friend bool operator==(const Matrix & lhs, const Matrix & rhs) {
        return lhs.elements_ == rhs.elements_;
    }

    friend bool almost_equal(const Matrix & lhs, const Matrix & rhs) {
        using rtc::almost_equal;

        for (auto row = 0U; row < N; ++row) {
            for (auto col = 0U; col < N; ++col) {
                if (!almost_equal(lhs(row, col), rhs(row, col))) {
                    return false;
                }
            }
        }
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix & t) {
        os << "\nmatrix" << N << "x" << N << "({\n";
        for (auto r = 0U; r < N; ++r) {
            os << "    {";
            for (auto c = 0U; c < N; ++c) {
                os << boost::format("%10.6f") % t(r, c) << ", ";
            }
            os << "},\n";
        }
        os << "})";
        return os;
    }

    // Fluent API support:
    Matrix then(const Matrix & m) {
        *this = m * *this;
        return *this;
    }

private:
    std::array<std::array<T, N>, N> elements_ {};
};

template <typename T>
using Matrix4x4 = Matrix<T, 4>;

template <typename T, unsigned int N>
inline bool almost_equal(const Matrix<T, N> & lhs, const Matrix<T, N> & rhs) {
    using rtc::almost_equal;
    return almost_equal(lhs.x(), rhs.x())
           && almost_equal(lhs.y(), rhs.y())
           && almost_equal(lhs.z(), rhs.z())
           && almost_equal(lhs.w(), rhs.w());
}

template <typename T, unsigned int R, unsigned int C>
T mrc(const Matrix<T, 2> & a, const Matrix<T, 2> & b) {
    return a(R, 0) * b(0, C) +
           a(R, 1) * b(1, C);
}

template <typename T, unsigned int R, unsigned int C>
T mrc(const Matrix<T, 3> & a, const Matrix<T, 3> & b) {
    return a(R, 0) * b(0, C) +
           a(R, 1) * b(1, C) +
           a(R, 2) * b(2, C);
}

template <typename T, unsigned int R, unsigned int C>
T mrc(const Matrix<T, 4> & a, const Matrix<T, 4> & b) {
    return a(R, 0) * b(0, C) +
           a(R, 1) * b(1, C) +
           a(R, 2) * b(2, C) +
           a(R, 3) * b(3, C);
}

template <typename T, unsigned int R, typename TupleType>
T mrc(const Matrix<T, 4> & a, const TupleType & t) {
    return a(R, 0) * t(0) +
           a(R, 1) * t(1) +
           a(R, 2) * t(2) +
           a(R, 3) * t(3);
}

template <typename T>
inline Matrix<T, 2> operator*(const Matrix<T, 2> & a, const Matrix<T, 2> & b)
{
    return {
        {mrc<T,0,0>(a, b), mrc<T,0,1>(a, b)},
        {mrc<T,1,0>(a, b), mrc<T,1,1>(a, b)},
    };
}

template <typename T>
inline Matrix<T, 3> operator*(const Matrix<T, 3> & a, const Matrix<T, 3> & b)
{
    return {
        {mrc<T,0,0>(a, b), mrc<T,0,1>(a, b), mrc<T,0,2>(a, b)},
        {mrc<T,1,0>(a, b), mrc<T,1,1>(a, b), mrc<T,1,2>(a, b)},
        {mrc<T,2,0>(a, b), mrc<T,2,1>(a, b), mrc<T,2,2>(a, b)},
    };
}

template <typename T>
inline Matrix<T, 4> operator*(const Matrix<T, 4> & a, const Matrix<T, 4> & b)
{
    return {
        {mrc<T,0,0>(a, b), mrc<T,0,1>(a, b), mrc<T,0,2>(a, b), mrc<T,0,3>(a, b)},
        {mrc<T,1,0>(a, b), mrc<T,1,1>(a, b), mrc<T,1,2>(a, b), mrc<T,1,3>(a, b)},
        {mrc<T,2,0>(a, b), mrc<T,2,1>(a, b), mrc<T,2,2>(a, b), mrc<T,2,3>(a, b)},
        {mrc<T,3,0>(a, b), mrc<T,3,1>(a, b), mrc<T,3,2>(a, b), mrc<T,3,3>(a, b)},
    };
}

template <typename T, typename TupleType>
inline TupleType operator*(const Matrix<T, 4> & a, const TupleType & t) {
    return {
        mrc<T,0>(a, t),
        mrc<T,1>(a, t),
        mrc<T,2>(a, t),
        mrc<T,3>(a, t),
    };
}

template <typename T>
inline Matrix<T, 4> transpose(const Matrix<T, 4> & m) {
    return {
        {m(0, 0), m(1, 0), m(2, 0), m(3, 0)},
        {m(0, 1), m(1, 1), m(2, 1), m(3, 1)},
        {m(0, 2), m(1, 2), m(2, 2), m(3, 2)},
        {m(0, 3), m(1, 3), m(2, 3), m(3, 3)},
    };
}

template <typename T>
inline T determinant(const Matrix<T, 2> & m) {
    return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
}

template <typename T, unsigned int N>
inline auto submatrix(const Matrix<T, N> & m, unsigned int row, unsigned int column) {
    std::vector<T> el;
    el.reserve((N - 1) * (N - 1));
    for (auto r = 0U; r < N; ++r) {
        if (r != row) {
            for (auto c = 0U; c < N; ++c) {
                if (c != column) {
                    el.push_back(m(r, c));
                }
            }
        }
    }
    return Matrix<T, N - 1> {el};
}

template <typename T, unsigned int N>
inline auto minor(const Matrix<T, N> & m, unsigned int row, unsigned int column) {
    const auto b = submatrix(m, row, column);
    return determinant(b);
}

template <typename T, unsigned int N>
inline auto cofactor(const Matrix<T, N> & m, unsigned int row, unsigned int column) {
    auto factor = -2 * ((static_cast<int>(row + column)) % 2) + 1;
    return minor(m, row, column) * factor;
}

template <typename T, unsigned int N>
inline T determinant(const Matrix<T, N> & m) {
    auto det = T(0);
    for (auto c = 0U; c < N; ++c) {
        det += m(0, c) * cofactor(m, 0, c);
    }
    return det;
}

template <typename T, unsigned int N>
inline bool is_invertible(const Matrix<T, N> & m) {
    return determinant(m) != T(0);
}

template <typename T, unsigned int N>
inline Matrix<T, N> inverse(const Matrix<T, N> & m) {
    auto det = determinant(m);

    Matrix<T, N> m2;
    for (auto row = 0U; row < N; ++row) {
        for (auto col = 0U; col < N; ++col) {
            auto c = cofactor(m, row, col);
            // do transpose by swapping r, c here:
            m2.set(col, row, c / det);
        }
    }
    return m2;
}

// Factory functions:
template <typename T>
inline auto matrix2x2(const std::initializer_list<std::initializer_list<T>> & elements) {
    return Matrix<T, 2> {elements};
}

template <typename T>
inline auto matrix3x3(const std::initializer_list<std::initializer_list<T>> & elements) {
    return Matrix<T, 3> {elements};
}

template <typename T>
inline auto matrix4x4(const std::initializer_list<std::initializer_list<T>> & elements) {
    return Matrix<T, 4> {elements};
}

template <typename T=fp_t>
inline auto identity4x4() {
    return Matrix<T, 4> {
        {T(1), T(0), T(0), T(0)},
        {T(0), T(1), T(0), T(0)},
        {T(0), T(0), T(1), T(0)},
        {T(0), T(0), T(0), T(1)},
    };
}

} // namespace rtc

#endif // RTC_LIB_MATRICES_H
