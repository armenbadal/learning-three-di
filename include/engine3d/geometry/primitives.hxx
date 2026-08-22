#pragma once

#include "engine3d/geometry/mesh.hxx"

namespace e3d::geometry::primitives {

[[nodiscard]]
mesh cube(float size = 1.0F);

[[nodiscard]]
mesh plane(float width, float depth);

} // namespace e3d::geometry::primitives
