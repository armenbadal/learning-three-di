#pragma once

#include "engine3d/math/vector2.hxx"
#include "engine3d/math/vector3.hxx"
#include "engine3d/renderer/colour.hxx"
#include "engine3d/renderer/framebuffer.hxx"
#include "engine3d/geometry/vertex.hxx"

#include <tuple>

namespace engine3d::renderer {

using math::vector2;
using math::vector3;
using geometry::vertex2d;

class rasterizer {
public:
    explicit rasterizer(framebuffer& fb) noexcept;

    void draw_line(vector2 p0, vector2 p1, colour c);
    void draw_triangle(vector2 p0, vector2 p1, vector2 p2, colour c);
    void draw_filled_triangle(vector2 p0, vector2 p1, vector2 p2, colour c);
    void draw_filled_triangle(vertex2d v0, vertex2d v1, vertex2d v2);

private:
    framebuffer& _framebuffer;
};

} // namespace engine3d::renderer
