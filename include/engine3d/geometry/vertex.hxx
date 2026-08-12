#pragma once

#include "engine3d/math/vector2.hxx"
#include "engine3d/math/vector4.hxx"
#include "engine3d/graphics/colour.hxx"

namespace e3d::geometry {

struct vertex2d {
    e3d::math::vector2 position;
    e3d::graphics::colour colour;
};

} // namespace e3d::geometry
