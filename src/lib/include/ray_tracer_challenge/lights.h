#ifndef RTC_LIB_LIGHTS_H
#define RTC_LIB_LIGHTS_H

#include "tuples.h"
#include "color.h"

namespace rtc {

class PointLight {
public:
    PointLight() = default;
    PointLight(Point const & position, Color const & intensity) :
        position_{position}, intensity_{intensity} {}

    auto operator<=>(PointLight const &) const = default;

    auto position() const { return position_; }
    auto intensity() const { return intensity_; }

private:
    Point position_;
    Color intensity_;
};

inline auto point_light(Point const & position, Color const & intensity) {
    return PointLight {position, intensity};
}

} // namespace rtc

#endif // RTC_LIB_LIGHTS_H
