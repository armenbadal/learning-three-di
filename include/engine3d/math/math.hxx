#pragma once

#include <algorithm>
#include <cmath>

namespace e3d::math {

inline constexpr float epsilon = 1e-5f;

inline bool almost_equal(float a, float b, float tolerance = epsilon) noexcept
{
    if( a == b )
        return true;
    if( !std::isfinite(a) || !std::isfinite(b) )
        return false;

    const float scale = std::max(1.0F, std::max(std::fabs(a), std::fabs(b)));
    return std::fabs(a - b) <= tolerance * scale;
}

} // namespace e3d::math
