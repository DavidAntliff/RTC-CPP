#ifndef RTC_LIB_CAMERA_H
#define RTC_LIB_CAMERA_H

#include <algorithm>

#include "./math.h"
#include "tuples.h"
#include "matrices.h"
#include "transformations.h"
#include "rays.h"
#include "world.h"
#include "canvas.h"

namespace rtc {

class Camera {
public:
    Camera() = default;
    Camera(unsigned int hsize, unsigned int vsize, fp_t field_of_view) :
        hsize_(hsize), vsize_(vsize), field_of_view_(field_of_view) {
        calc_pixel_size_();
    }

    auto hsize() const { return hsize_; }
    auto vsize() const { return vsize_; }
    auto field_of_view() const { return field_of_view_; }

    auto const & transform() const { return transform_; }
    auto & transform() { return transform_; }  // TODO: remove
    void set_transform(decltype(identity4x4()) const & transform) {
        transform_ = transform;
    }

    auto pixel_size() const { return pixel_size_; }
    auto half_width() const { return half_width_; }
    auto half_height() const { return half_height_; }

private:
    void calc_pixel_size_() {
        auto const half_view {tan(field_of_view_ / 2.0)};
        auto const aspect_ratio {static_cast<fp_t>(hsize_) / static_cast<fp_t>(vsize_)};
        if (aspect_ratio >= 1.0) {
            half_width_ = half_view;
            half_height_ = half_view / aspect_ratio;
        } else {
            half_width_ = half_view * aspect_ratio;
            half_height_ = half_view;
        }
        pixel_size_ = (half_width_ * 2.0) / hsize_;
    }

private:
    unsigned int hsize_ {};
    unsigned int vsize_ {};
    fp_t field_of_view_ {};
    decltype(identity4x4()) transform_ {identity4x4()};

    fp_t half_width_ {};
    fp_t half_height_ {};
    fp_t pixel_size_ {};
};

inline auto camera(unsigned int hsize, unsigned int vsize, fp_t field_of_view) {
    return Camera {hsize, vsize, field_of_view};
}

template <typename T=fp_t>
inline auto ray_for_pixel(Camera const & camera, unsigned int px, unsigned int py) {
    // the offset from the edge of the canvas to the pixel's center
    auto const xoffset {(px + 0.5) * camera.pixel_size()};
    auto const yoffset {(py + 0.5) * camera.pixel_size()};

    // the untransformed coordinates of the pixel in world space.
    // (the camera looks toward -Z, so +X is to the *left*)
    auto const world_x {camera.half_width() - xoffset};
    auto const world_y {camera.half_height() - yoffset};

    // using the camera matrix, transform the canvas point and the origin,
    // and then compute the ray's direction vector.
    // (the canvas is at Z=-1)
    auto const inverse_camera_transform {inverse(camera.transform())};
    auto const pixel {inverse_camera_transform * point(world_x, world_y, -1.0)};
    auto const origin {inverse_camera_transform * point(0.0, 0.0, 0.0)};
    auto const direction {normalize(pixel - origin)};

    return ray(origin, direction);
}

template <typename T>
inline auto render(Camera const & camera, World<T> const & world) {
    auto image {canvas(camera.hsize(), camera.vsize())};
    for (unsigned int y {0}; y < camera.vsize(); ++y) {
        for (unsigned int x {0}; x < camera.hsize(); ++x) {
            auto const ray {ray_for_pixel(camera, x, y)};
            auto const color {color_at(world, ray)};
            write_pixel(image, x, y, color);
        }
    }
    return image;
}

}

#endif // RTC_LIB_CAMERA_H
