#pragma once

#include "engine3d/math/vector2.hxx"
#include "engine3d/math/vector3.hxx"
#include "engine3d/graphics/colour.hxx"

namespace e3d::geometry {

struct vertex2d {
    e3d::math::vector2 position;
    e3d::graphics::colour colour;
};

struct vertex3d {
    e3d::math::vector3 position;
    e3d::graphics::colour colour{e3d::graphics::white};
};

} // namespace e3d::geometry
