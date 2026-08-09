#pragma once

#include "engine3d/math/vector2.hxx"
#include "engine3d/renderer/colour.hxx"

namespace engine3d::renderer {

struct vertex2d {
    math::vector2 _position;
    colour _colour;
};

} // namespace engine3d::renderer
