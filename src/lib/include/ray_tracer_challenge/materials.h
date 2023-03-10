#ifndef RTC_LIB_MATERIALS_H
#define RTC_LIB_MATERIALS_H

#include "tuples.h"
#include "color.h"
#include "lights.h"

namespace rtc {

template <typename T>
class Material {
public:
    Material() = default;

    bool operator==(Material const &) const = default;

    auto color() const { return color_; }
    auto ambient() const { return ambient_; }
    auto diffuse() const { return diffuse_; }
    auto specular() const { return specular_; }
    auto shininess() const { return shininess_; }

    void set_color(Color<Tuple<T>> const & color) { color_ = color; }
    void set_ambient(T value) { ambient_ = value; }
    void set_diffuse(T value) { diffuse_ = value; }
    void set_specular(T value) { specular_ = value; }
    void set_shininess(T value) { shininess_ = value; }


private:
    Color<Tuple<T>> color_ {1.0, 1.0, 1.0};
    T ambient_ {0.1};
    T diffuse_ {0.9};
    T specular_ {0.9};
    T shininess_ {200.0};
};

template <typename T=fp_t>
inline auto material() {
    return Material<T> {};
}

template <typename T=fp_t>
inline auto lighting(Material<T> const & material,
                     PointLight<T> const & light,
                     Point<T> const & point,
                     Vector<T> const & eyev,
                     Vector<T> const & normalv) {

    // Combine the surface color with the light's color/intensity
    auto effective_color = material.color() * light.intensity();

    // Find the direction to the light source
    auto lightv = normalize(light.position() - point);

    // Compute the ambient contribution
    auto ambient = effective_color * material.ambient();

    Color<Tuple<T>> diffuse {};
    Color<Tuple<T>> specular {};

    // light_dot_normal represents the cosine of the angle between the
    // light vector and the normal vector. A negative number means the
    // light is on the other side of the surface.
    auto light_dot_normal = dot(lightv, normalv);
    if (light_dot_normal < 0) {
        diffuse = color(0.0, 0.0, 0.0);  // black
        specular = color(0.0, 0.0, 0.0);  // black
    } else {
        // Compute the diffuse contribution
        diffuse = effective_color * material.diffuse() * light_dot_normal;

        // reflect_dot_eye represents the cosine of the angle between the
        // reflection vector and the eye vector. A negative number means the
        // light reflects away from the eye.
        auto reflectv = reflect(-lightv, normalv);
        auto reflect_dot_eye = dot(reflectv, eyev);

        if (reflect_dot_eye <= 0) {
            specular = color(0.0, 0.0, 0.0);
        } else {
            // Compute the specular contribution
            auto factor = pow(reflect_dot_eye, material.shininess());
            specular = light.intensity() * material.specular() * factor;
        }
    }

    return ambient + diffuse + specular;
}

} // namespace rtc

#endif // RTC_LIB_MATERIALS_H
