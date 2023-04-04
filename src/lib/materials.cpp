#include "ray_tracer_challenge/materials.h"
#include "ray_tracer_challenge/color.h"
#include "ray_tracer_challenge/shapes.h"
#include "ray_tracer_challenge/patterns.h"

namespace rtc {

Color lighting(Material const & material,
               Shape const & shape,
               PointLight const & light,
               Point const & point,
               Vector const & eyev,
               Vector const & normalv,
               bool in_shadow) {

    auto const pattern {material.pattern()};
    auto const material_color {pattern ? pattern_at_shape(*pattern, shape, point) : material.color()};

    // Combine the surface color with the light's color/intensity
    auto const effective_color = material_color * light.intensity();

    // Find the direction to the light source
    auto const lightv = normalize(light.position() - point);

    // Compute the ambient contribution
    auto const ambient = effective_color * material.ambient();

    if (in_shadow) {
        return ambient;
    }

    Color diffuse {};
    Color specular {};

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
