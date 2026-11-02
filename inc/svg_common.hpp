#ifndef SVG_COMMON_HPP
#define SVG_COMMON_HPP

#include <stdint.h>

namespace svg
{
constexpr int32_t font_size = 16;

// Rotate a 2D offset (dx, dy) by the given angle (0, 90, 180, 270 degrees).
// Uses SVG rotation convention: rotate(θ, cx, cy) maps offset (dx, dy) as follows:
//   0°   → (dx, dy)        90°  → (-dy, dx)
//   180° → (-dx, -dy)      270° → (dy, -dx)
inline void rotate_offset(int32_t &dx, int32_t &dy, int32_t rotation)
{
    int32_t r = ((rotation % 360) + 360) % 360;
    int32_t new_dx = dx, new_dy = dy;
    switch (r)
    {
        case 90:  new_dx = -dy; new_dy = dx;  break;
        case 180: new_dx = -dx; new_dy = -dy; break;
        case 270: new_dx = dy;  new_dy = -dx; break;
        default:  return;
    }
    dx = new_dx;
    dy = new_dy;
}
}

#endif /* SVG_COMMON_HPP */

