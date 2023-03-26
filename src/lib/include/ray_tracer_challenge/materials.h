#ifndef RTC_LIB_MATERIALS_H
#define RTC_LIB_MATERIALS_H

#include "tuples.h"
#include "color.h"
#include "lights.h"
#include "patterns.h"

namespace rtc {

template <typename T>
class Material {
public:
    using value_t = T;

    Material() = default;
    Material(Color<T> color,
             T ambient,
             T diffuse,
             T specular,
             T shininess) :
             color_{color}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess} {}

    auto operator<=>(Material const &) const = default;

    auto color() const { return color_; }
    auto ambient() const { return ambient_; }
    auto diffuse() const { return diffuse_; }
    auto specular() const { return specular_; }
    auto shininess() const { return shininess_; }

    void set_color(Color<T> const & color) { color_ = color; }
    void set_ambient(T value) { ambient_ = value; }
    void set_diffuse(T value) { diffuse_ = value; }
    void set_specular(T value) { specular_ = value; }
    void set_shininess(T value) { shininess_ = value; }

    std::optional<StripePattern<T>> pattern() const { return pattern_; }
    void set_pattern(StripePattern<T> const & pattern) { pattern_ = pattern; }

private:
    Color<T> color_ {1.0, 1.0, 1.0};
    T ambient_ {0.1};
    T diffuse_ {0.9};
    T specular_ {0.9};
    T shininess_ {200.0};

    std::optional<StripePattern<T>> pattern_ {};
};

template <typename T=fp_t>
inline auto material() {
    return Material<T> {};
}

template <typename T=fp_t>
inline auto material(Color<T> const & color,
                     T ambient,
                     T diffuse,
                     T specular,
                     T shininess) {
    return Material<T> {color, ambient, diffuse, specular, shininess};
}

template <typename T=fp_t>
inline auto lighting(Material<T> const & material,
                     Shape<T> const & object,
                     PointLight<T> const & light,
                     Point<T> const & point,
                     Vector<T> const & eyev,
                     Vector<T> const & normalv,
                     bool in_shadow) {

    auto const pattern {material.pattern()};
    auto const material_color {pattern ? stripe_at_object(*pattern, object, point) : material.color()};

    // Combine the surface color with the light's color/intensity
    auto const effective_color = material_color * light.intensity();

    // Find the direction to the light source
    auto const lightv = normalize(light.position() - point);

    // Compute the ambient contribution
    auto const ambient = effective_color * material.ambient();

    if (in_shadow) {
        return ambient;
    }

    Color<T> diffuse {};
    Color<T> specular {};

    // light_dot_normal represents the cosine of the angle between the
    // light vector and the normal vector. A negative number means the
    // light is on the other side of the surface.
    auto const light_dot_normal = dot(lightv, normalv);
    if (light_dot_normal < 0) {
        diffuse = color(0.0, 0.0, 0.0);  // black
        specular = color(0.0, 0.0, 0.0);  // black
    } else {
        // Compute the diffuse contribution
        diffuse = effective_color * material.diffuse() * light_dot_normal;

        // reflect_dot_eye represents the cosine of the angle between the
        // reflection vector and the eye vector. A negative number means the
        // light reflects away from the eye.
        auto const reflectv = reflect(-lightv, normalv);
        auto const reflect_dot_eye = dot(reflectv, eyev);

        if (reflect_dot_eye <= 0) {
            specular = color(0.0, 0.0, 0.0);
        } else {
            // Compute the specular contribution
            auto const factor = pow(reflect_dot_eye, material.shininess());
            specular = light.intensity() * material.specular() * factor;
        }
    }

    return ambient + diffuse + specular;
}

} // namespace rtc

#endif // RTC_LIB_MATERIALS_H
