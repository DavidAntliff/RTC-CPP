#ifndef RTC_LIB_SPHERES_H
#define RTC_LIB_SPHERES_H

#include "./math.h"
#include "matrices.h"

namespace rtc {

template <typename T=fp_t>
class Sphere {
public:
    using matrix_t = Matrix<T, 4>;

    Sphere() = default;
    Sphere(int id) : id_{id} {}

    matrix_t const & transform() const { return transform_; }

    void set_transform(matrix_t const & m) {
        transform_ = m;
    }

private:
    int id_ {};
    matrix_t transform_ {identity4x4()};
};

inline auto sphere(int id) {
    return Sphere {id};
}

} // namespace rtc

#endif // RTC_LIB_SPHERES_H
