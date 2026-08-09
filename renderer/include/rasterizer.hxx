#pragma once

#include "colour.hxx"
#include "framebuffer.hxx"
#include "vector2.hxx"
#include "vector3.hxx"
#include "vertex2d.hxx"

#include <tuple>

namespace renderer {

class rasterizer {
public:
    rasterizer(framebuffer& fb);

    void draw_line(math::vector2 p0, math::vector2 p1, colour c);
    void draw_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c);
    void draw_filled_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c);
    void draw_filled_triangle(vertex2d v0, vertex2d v1, vertex2d v2);

    static std::tuple<math::vector2, math::vector2> bounding_box(math::vector2 p0, math::vector2 p1, math::vector2 p2);
    static math::vector3 barycentric(math::vector2 p, math::vector2 a, math::vector2 b, math::vector2 c);

private:
    framebuffer& _framebuffer;
};

} // namespace renderer
