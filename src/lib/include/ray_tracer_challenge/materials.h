#ifndef RTC_LIB_MATERIALS_H
#define RTC_LIB_MATERIALS_H

#include "tuples.h"
#include "color.h"
#include "lights.h"
#include "patterns.h"

namespace rtc {

class Shape;

class Material {
public:
    Material() = default;
    Material(Color color,
             fp_t ambient,
             fp_t diffuse,
             fp_t specular,
             fp_t shininess) :
             color_{color}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, shininess_{shininess} {}

    // https://stackoverflow.com/a/43263477
    ~Material() = default;
    Material(Material const & other)
        : color_{other.color_},
          ambient_{other.ambient_},
          diffuse_{other.diffuse_},
          specular_{other.specular_},
          shininess_{other.shininess_},
          pattern_{other.pattern_ ? other.pattern_->clone() : nullptr} {}
    Material(Material &&) = default;
    Material& operator=(Material const & other) {
        pattern_ = other.pattern_ ? other.pattern_->clone() : nullptr;
        color_ = other.color_;
        ambient_ = other.ambient_;
        diffuse_ = other.diffuse_;
        specular_ = other.specular_;
        shininess_ = other.shininess_;
        return *this;
    }
    Material& operator=(Material &&) = default;

    //auto operator<=>(Material const &) const = default;
    friend bool operator==(Material const & lhs, Material const & rhs) {
        auto equal = lhs.color_ == rhs.color_ &&
                     lhs.ambient_ == rhs.ambient_ &&
                     lhs.diffuse_ == rhs.diffuse_ &&
                     lhs.specular_ == rhs.specular_ &&
                     lhs.shininess_ == rhs.shininess_;

        // Compare patterns, if not null
        if (equal) {
            if (lhs.pattern_) {  // lhs is not null
                if (rhs.pattern_) {  // rhs is not null
                    // then compare the patterns
                    equal = *lhs.pattern_ == *rhs.pattern_;
                } else {
                    // rhs is null, must be unequal
                    equal = false;
                }
            } else {  // lhs is null
                if (rhs.pattern_) {  // rhs is not null
                    equal = false;  // must be unequal
                }
                // else rhs is null, equal remains true
            }
        }
        return equal;
    }

    auto color() const { return color_; }
    auto ambient() const { return ambient_; }
    auto diffuse() const { return diffuse_; }
    auto specular() const { return specular_; }
    auto shininess() const { return shininess_; }

    void set_color(Color const & color) { color_ = color; }
    void set_ambient(fp_t value) { ambient_ = value; }
    void set_diffuse(fp_t value) { diffuse_ = value; }
    void set_specular(fp_t value) { specular_ = value; }
    void set_shininess(fp_t value) { shininess_ = value; }

    Pattern * pattern() const { return pattern_.get(); }
    void set_pattern(Pattern const & pattern) {
        pattern_ = pattern.clone();
    }

private:
    Color color_ {1.0, 1.0, 1.0};
    fp_t ambient_ {0.1};
    fp_t diffuse_ {0.9};
    fp_t specular_ {0.9};
    fp_t shininess_ {200.0};

    std::unique_ptr<Pattern> pattern_ {};
};

inline auto material() {
    return Material {};
}

inline auto material(Color const & color,
                     fp_t ambient,
                     fp_t diffuse,
                     fp_t specular,
                     fp_t shininess) {
    return Material {color, ambient, diffuse, specular, shininess};
}

Color lighting(Material const & material,
               Shape const & shape,
               PointLight const & light,
               Point const & point,
               Vector const & eyev,
               Vector const & normalv,
               bool in_shadow);

} // namespace rtc

#endif // RTC_LIB_MATERIALS_H
