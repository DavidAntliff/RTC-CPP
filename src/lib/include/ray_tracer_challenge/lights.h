#ifndef RTC_LIB_LIGHTS_H
#define RTC_LIB_LIGHTS_H

#include "tuples.h"
#include "color.h"

namespace rtc {

template <typename T=fp_t>
class PointLight {
public:
    PointLight() = default;
    PointLight(Point<T> const & position, Color<Tuple<T>> const & intensity) :
        position_{position}, intensity_{intensity} {}

    auto position() const { return position_; }
    auto intensity() const { return intensity_; }

private:
    Point<T> position_;
    Color<Tuple<T>> intensity_;
};

template <typename T=fp_t>
inline auto point_light(Point<T> const & position, Color<Tuple<T>> const & intensity) {
    return PointLight<T> {position, intensity};
}

} // namespace rtc

#endif // RTC_LIB_LIGHTS_H
