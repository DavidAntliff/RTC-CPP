#ifndef RTC_LIB_MATRICES_H
#define RTC_LIB_MATRICES_H

#include <array>
#include <initializer_list>
#include <ostream>
#include <vector>
#include <optional>

#include <boost/format.hpp>

#include "./math.h"
#include "tuples.h"

namespace rtc {

// Square Matrices only
template <unsigned int N=0>
class Matrix {
public:
    using value_t = fp_t;

    Matrix() = default;

    explicit Matrix(std::initializer_list<std::initializer_list<value_t>> const & elements)
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
    explicit Matrix(std::vector<value_t> const & elements) {
        assert(elements.size() == N * N);
        typename std::vector<value_t>::const_iterator iter = elements.begin();
        for (auto r = 0U; r < N; ++r) {
            for (auto c = 0U; c < N; ++c) {
                elements_[r][c] = *iter;
                ++iter;
            }
        }
    }

    unsigned int dim() const { return N; }

    std::optional<value_t> at(unsigned int row, unsigned int column) const {
        if (row >= N)
            return std::nullopt;
        if (column >= N)
            return std::nullopt;
        return operator()(row, column);
    }

    // No bounds checking - undefined behaviour if row/column are out of bounds
    value_t operator()(unsigned int row, unsigned int column) const {
        return elements_[row][column];
    }

    void unsafe_set(unsigned int row, unsigned int column, value_t value) {
        elements_[row][column] = value;
    }

    void set(unsigned int row, unsigned int column, value_t value) {
        if (row >= N)
            return;
        if (column >= N)
            return;
        unsafe_set(row, column, value);
    }

    // Strict equality for floating point types
    auto operator<=>(Matrix const &) const = default;

    friend bool almost_equal(Matrix const & lhs, Matrix const & rhs) {
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

    friend std::ostream& operator<<(std::ostream& os, Matrix const & t) {
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
    Matrix then(Matrix const & m) {
        *this = m * *this;
        return *this;
    }

private:
    std::array<std::array<value_t, N>, N> elements_ {};
};

template <unsigned int N>
inline bool almost_equal(Matrix<N> const & lhs, Matrix<N> const & rhs) {
    using rtc::almost_equal;
    return almost_equal(lhs.x(), rhs.x())
           && almost_equal(lhs.y(), rhs.y())
           && almost_equal(lhs.z(), rhs.z())
           && almost_equal(lhs.w(), rhs.w());
}

template <unsigned int R, unsigned int C>
inline auto mrc(Matrix<2> const & a, Matrix<2> const & b) {
    return a(R, 0) * b(0, C) +
           a(R, 1) * b(1, C);
}

template <unsigned int R, unsigned int C>
inline auto mrc(Matrix<3> const & a, Matrix<3> const & b) {
    return a(R, 0) * b(0, C) +
           a(R, 1) * b(1, C) +
           a(R, 2) * b(2, C);
}

template <unsigned int R, unsigned int C>
inline auto mrc(Matrix<4> const & a, Matrix<4> const & b) {
    return a(R, 0) * b(0, C) +
           a(R, 1) * b(1, C) +
           a(R, 2) * b(2, C) +
           a(R, 3) * b(3, C);
}

template <unsigned int R>
inline auto mrc(Matrix<4> const & a, Tuple const & t) {
    return a(R, 0) * t(0) +
           a(R, 1) * t(1) +
           a(R, 2) * t(2) +
           a(R, 3) * t(3);
}

inline Matrix<2> operator*(Matrix<2> const & a, Matrix<2> const & b)
{
    return Matrix<2> {
        {mrc<0,0>(a, b), mrc<0,1>(a, b)},
        {mrc<1,0>(a, b), mrc<1,1>(a, b)},
    };
}

inline Matrix<3> operator*(Matrix<3> const & a, Matrix<3> const & b)
{
    return Matrix<3> {
        {mrc<0,0>(a, b), mrc<0,1>(a, b), mrc<0,2>(a, b)},
        {mrc<1,0>(a, b), mrc<1,1>(a, b), mrc<1,2>(a, b)},
        {mrc<2,0>(a, b), mrc<2,1>(a, b), mrc<2,2>(a, b)},
    };
}

inline Matrix<4> operator*(Matrix<4> const & a, Matrix<4> const & b) {
    return Matrix<4> {
        {mrc<0,0>(a, b), mrc<0,1>(a, b), mrc<0,2>(a, b), mrc<0,3>(a, b)},
        {mrc<1,0>(a, b), mrc<1,1>(a, b), mrc<1,2>(a, b), mrc<1,3>(a, b)},
        {mrc<2,0>(a, b), mrc<2,1>(a, b), mrc<2,2>(a, b), mrc<2,3>(a, b)},
        {mrc<3,0>(a, b), mrc<3,1>(a, b), mrc<3,2>(a, b), mrc<3,3>(a, b)},
    };
}

// Experiment using Loop Interchange:
// https://johnnysswlab.com/for-software-performance-the-way-data-is-accessed-matters/
// However this does not seem to be any faster than the initialisation using mrc() above.
//template <typename T>
//inline Matrix<4> operator*(Matrix<4> const & a, Matrix<4> const & b) {
//    Matrix<4> m {};
//    for (int i = 0; i < 4; ++i) {
//        for (int k = 0; k < 4; ++k) {  // loop interchange
//            for (int j = 0; j < 4; ++j) {
//                m.unsafe_set(i, j, m(i, j) + a(i, k) * b(k, j));
//            }
//        }
//    }
//    return m;
//}

inline Tuple operator*(Matrix<4> const & a, Tuple const & t) {
    return {
        mrc<0>(a, t),
        mrc<1>(a, t),
        mrc<2>(a, t),
        mrc<3>(a, t),
    };
}

inline Matrix<4> transpose(Matrix<4> const & m) {
    return Matrix<4> {
        {m(0, 0), m(1, 0), m(2, 0), m(3, 0)},
        {m(0, 1), m(1, 1), m(2, 1), m(3, 1)},
        {m(0, 2), m(1, 2), m(2, 2), m(3, 2)},
        {m(0, 3), m(1, 3), m(2, 3), m(3, 3)},
    };
}

inline auto determinant(Matrix<2> const & m) {
    return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
}

// ~20 seconds for chapter8.cpp, release build, 1024x768
template <unsigned int N>
inline auto submatrix_slow(Matrix<N> const & m, unsigned int row, unsigned int column) {
    std::vector<typename decltype(m)::value_type> el;
    el.reserve((N - 1) * (N - 1));
    for (auto r = 0U; r < N; ++r) {
        if (r != row) {
            for (auto c = 0U; c < N; ++c) {
                if (c != column) {
                    el.emplace_back(m(r, c));
                }
            }
        }
    }
    return Matrix<N - 1> {el};
}

// ~2 seconds for chapter8.cpp, release build, 1024x768
template <unsigned int N>
inline auto submatrix(Matrix<N> const & m, unsigned int row, unsigned int column) {
    Matrix<N - 1> s{};
    for (auto r = 0U, sr = 0U; r < N; ++r) {
        if (r != row) {
            for (auto c = 0U, sc = 0U; c < N; ++c) {
                if (c != column) {
                    s.unsafe_set(sr, sc, m(r, c));
                    ++sc;
                }
            }
            ++sr;
        }
    }
    return s;
}

template <unsigned int N>
inline auto minor(Matrix<N> const & m, unsigned int row, unsigned int column) {
    auto const b = submatrix(m, row, column);
    return determinant(b);
}

template <unsigned int N>
inline auto cofactor(Matrix<N> const & m, unsigned int row, unsigned int column) {
    auto const factor = -2 * ((static_cast<int>(row + column)) % 2) + 1;
    return minor(m, row, column) * factor;
}

template <unsigned int N>
inline auto determinant(Matrix<N> const & m) {
    fp_t det {0};
    for (auto c = 0U; c < N; ++c) {
        det += m(0, c) * cofactor(m, 0, c);
    }
    return det;
}

template <unsigned int N>
inline bool is_invertible(Matrix<N> const & m) {
    return determinant(m) != 0;
}

template <unsigned int N>
inline auto inverse(Matrix<N> const & m) {
    auto const det = determinant(m);

    Matrix<N> m2;
    for (auto row = 0U; row < N; ++row) {
        for (auto col = 0U; col < N; ++col) {
            auto const c = cofactor(m, row, col);
            // do transpose by swapping r, c here:
            m2.unsafe_set(col, row, c / det);
        }
    }
    return m2;
}

// Factory functions:
inline auto matrix2x2(std::initializer_list<std::initializer_list<fp_t>> const & elements) {
    return Matrix<2> {elements};
}

inline auto matrix3x3(std::initializer_list<std::initializer_list<fp_t>> const & elements) {
    return Matrix<3> {elements};
}

inline auto matrix4x4(std::initializer_list<std::initializer_list<fp_t>> const & elements) {
    return Matrix<4> {elements};
}

inline auto identity4x4() {
    return Matrix<4> {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    };
}

} // namespace rtc

#endif // RTC_LIB_MATRICES_H
