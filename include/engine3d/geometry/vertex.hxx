#pragma once

#include "engine3d/math/vector2.hxx"
#include "engine3d/math/vector4.hxx"
#include "engine3d/renderer/colour.hxx"

namespace engine3d::geometry {

struct vertex2d {
    engine3d::math::vector2 position;
    engine3d::renderer::colour colour;
};

struct clip_vertex {
    engine3d::math::vector4 position;
};

} // namespace engine3d::geometry
