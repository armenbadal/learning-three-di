#pragma once

#include "engine3d/graphics/colour.hxx"
#include "engine3d/geometry/mesh.hxx"

#include <array>

namespace e3d::geometry::primitives {

[[nodiscard]]
mesh cube(float size = 1.0F);

[[nodiscard]]
mesh cube(float size, const std::array<graphics::colour, 8>& colors);

[[nodiscard]]
mesh plane(float width, float depth);

} // namespace e3d::geometry::primitives
