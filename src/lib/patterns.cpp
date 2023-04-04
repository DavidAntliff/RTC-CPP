#include "ray_tracer_challenge/patterns.h"
#include "ray_tracer_challenge/color.h"
#include "ray_tracer_challenge/shapes.h"
#include "ray_tracer_challenge/tuples.h"

namespace rtc {

Color pattern_at_shape(Pattern const & pattern,
                       Shape const & shape,
                       Point const & world_point) {
    // Convert world-space point to object-space point:
    auto const object_point {inverse(shape.transform()) * world_point};
    return pattern_at(pattern, object_point);
}

} // namespace rtc
