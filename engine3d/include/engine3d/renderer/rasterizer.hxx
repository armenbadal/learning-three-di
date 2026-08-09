#pragma once

#include <engine3d/math/vector2.hxx>
#include <engine3d/math/vector3.hxx>
#include <engine3d/renderer/colour.hxx>
#include <engine3d/renderer/framebuffer.hxx>
#include <engine3d/renderer/vertex2d.hxx>

#include <tuple>

namespace engine3d::renderer {

class rasterizer {
public:
    explicit rasterizer(framebuffer& fb) noexcept;

    // Coordinates are in screen space; filled triangles sample pixel centres at x + 0.5, y + 0.5.
    void draw_line(math::vector2 p0, math::vector2 p1, colour c);
    void draw_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c);
    void draw_filled_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c);
    void draw_filled_triangle(vertex2d v0, vertex2d v1, vertex2d v2);

    static std::tuple<math::vector2, math::vector2> bounding_box(
        math::vector2 p0, math::vector2 p1, math::vector2 p2) noexcept;
    static math::vector3 barycentric(
        math::vector2 p, math::vector2 a, math::vector2 b, math::vector2 c) noexcept;

private:
    framebuffer& _framebuffer;
};

} // namespace engine3d::renderer
