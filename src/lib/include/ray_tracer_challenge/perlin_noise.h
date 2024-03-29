// Improved Perlin Noise
// Based on http://adrianb.io/2014/08/09/perlinnoise.html
// Original paper: https://mrl.cs.nyu.edu/~perlin/paper445.pdf
// Original code: https://mrl.cs.nyu.edu/~perlin/noise/

#ifndef RTC_LIB_PERLIN_NOISE_H
#define RTC_LIB_PERLIN_NOISE_H

#include <array>
#include <cstdint>
#include <cmath>
#include <optional>

namespace rtc {
namespace detail {

// Hash lookup table as defined by Ken Perlin.  This is a randomly
// arranged array of all numbers from 0-255 inclusive, repeated once.
static constexpr std::array<uint_fast8_t, 512> PERMUTATION = {
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
        7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
        190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
        203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
        20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
        166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
        220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
        63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
        200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
        186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118,
        126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
        189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
        70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
        98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,
        97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
        241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
        199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150,
        254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128,
        195, 78, 66, 215, 61, 156, 180,
        /* repeat */
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
        7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
        190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
        203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
        20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
        166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
        220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
        63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
        200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
        186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118,
        126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
        189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
        70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
        98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,
        97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
        241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
        199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150,
        254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128,
        195, 78, 66, 215, 61, 156, 180,
};

inline double grad(int hash, double x, double y, double z) {
    // Take the hashed value and take the first 4 bits of it (15 == 0b1111)
    int const h = hash & 15;

    // If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.
    double const u = h < 8 /* 0b1000 */ ? x : y;

    // In Ken Perlin's original implementation this was another conditional operator (?:).
    // Expand it for readability.
    double v {};

    if (h < 4 /* 0b0100 */) {
        // If the first and second significant bits are 0 set v = y
        v = y;
    } else if(h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/) {
        // If the first and second significant bits are 1 set v = x
        v = x;
    } else {
        // If the first and second significant bits are not equal (0/1, 1/0) set v = z
        v = z;
    }

    // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

inline double fade(double t) {
    // Fade function as defined by Ken Perlin.  This eases coordinate values
    // so that they will "ease" towards integral values.  This ends up smoothing
    // the final output.
    return t * t * t * (t * (t * 6 - 15) + 10);  // 6t^5 - 15t^4 + 10t^3
}

inline double lerp(double a, double b, double x) {
    return a + x * (b - a);
}

} // namespace detail

// TODO: what does `repeat` actually do?
class PerlinNoise {
public:
    explicit PerlinNoise(int repeat = -1) : repeat_{repeat} {}

    inline double perlin(double x, double y, double z) const {
        // If we have any repeat, change the coordinates to their "local" repetitions
        if (repeat_ > 0) {
            x = std::fmod(x, repeat_);
            y = std::fmod(y, repeat_);
            z = std::fmod(z, repeat_);
        }

        // Calculate the "unit cube" that the point asked will be located in
        // The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
        // plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.
        int const xi {static_cast<int>(std::floor(x)) & 255};
        int const yi {static_cast<int>(std::floor(y)) & 255};
        int const zi {static_cast<int>(std::floor(z)) & 255};

        // We also fade the location to smooth the result.
        double const xf {x - std::floor(x)};
        double const yf {y - std::floor(y)};
        double const zf {z - std::floor(z)};

        double const u {detail::fade(xf)};
        double const v {detail::fade(yf)};
        double const w {detail::fade(zf)};

        auto const & p {detail::PERMUTATION};

        int const aaa {p[p[p[    xi ]+    yi ]+    zi ]};
        int const aba {p[p[p[    xi ]+inc(yi)]+    zi ]};
        int const aab {p[p[p[    xi ]+    yi ]+inc(zi)]};
        int const abb {p[p[p[    xi ]+inc(yi)]+inc(zi)]};
        int const baa {p[p[p[inc(xi)]+    yi ]+    zi ]};
        int const bba {p[p[p[inc(xi)]+inc(yi)]+    zi ]};
        int const bab {p[p[p[inc(xi)]+    yi ]+inc(zi)]};
        int const bbb {p[p[p[inc(xi)]+inc(yi)]+inc(zi)]};

        using namespace detail;
        double x1, x2, y1, y2;

        // The gradient function calculates the dot product between a pseudorandom
        // gradient vector and the vector from the input coordinate to the 8
        // surrounding points in its unit cube.
        // This is all then lerped together as a sort of weighted average based on the faded (u,v,w)
        // values we made earlier.
        x1 = lerp(    grad (aaa, xf  , yf  , zf),
                      grad (baa, xf-1, yf  , zf),
                      u);
        x2 = lerp(    grad (aba, xf  , yf-1, zf),
                      grad (bba, xf-1, yf-1, zf),
                      u);
        y1 = lerp(x1, x2, v);

        x1 = lerp(    grad (aab, xf  , yf  , zf-1),
                      grad (bab, xf-1, yf  , zf-1),
                      u);
        x2 = lerp(    grad (abb, xf  , yf-1, zf-1),
                      grad (bbb, xf-1, yf-1, zf-1),
                      u);
        y2 = lerp (x1, x2, v);

        // For convenience we bound it to 0 - 1 (theoretical min/max before is -1 - 1)
        return (lerp(y1, y2, w) + 1.0) / 2.0;
    }

    inline double octave_perlin(double x, double y, double z, int octaves, double persistence) const {
        double total = 0;
        double frequency = 1;
        double amplitude = 1;
        double max_value = 0;            // Used for normalizing result to 0.0 - 1.0

        for(int i = 0; i < octaves; ++i) {
            total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;
            max_value += amplitude;
            amplitude *= persistence;
            frequency *= 2;
        }

        return total / max_value;
    }

private:
    inline int inc(int num) const {
        ++num;
        if (repeat_ > 0) {
            num = std::fmod(num, repeat_);
        }
        return num;
    }

private:
    int repeat_ {-1};
};

inline PerlinNoise perlin_noise(int repeat = -1) {
    return PerlinNoise(repeat);
}

} // namespace rtc

#endif // RTC_LIB_PERLIN_NOISE_H
