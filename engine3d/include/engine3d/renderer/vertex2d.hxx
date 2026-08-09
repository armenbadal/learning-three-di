#pragma once

#include "engine3d/math/vector2.hxx"
#include "engine3d/renderer/colour.hxx"

namespace engine3d::renderer {

struct vertex2d {
    engine3d::math::vector2 position;
    engine3d::renderer::colour colour;
};

} // namespace engine3d::renderer
