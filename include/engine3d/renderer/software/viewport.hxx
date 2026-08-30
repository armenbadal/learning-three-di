#pragma once

#include "engine3d/graphics/colour.hxx"
#include "engine3d/math/vector2.hxx"
#include "engine3d/math/vector3.hxx"
#include "engine3d/math/vector4.hxx"

namespace e3d::renderer {

struct screen_vertex {
    math::vector2 position;
    float depth;
    float inv_w{1.0F};
    graphics::colourf colour_over_w{};
};

screen_vertex viewport_transform(const math::vector3& ndc, float width, float height) noexcept;
math::vector3 perspective_divide(const math::vector4& p);

} // namespace e3d::renderer
