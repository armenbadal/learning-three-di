#pragma once

#include "colour.hxx"
#include "framebuffer.hxx"
#include "vector2.hxx"

namespace renderer {

class rasterizer {
public:
    rasterizer(framebuffer& fb);

    void draw_line(math::vector2 p0, math::vector2 p1, colour c);
    void draw_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c);
    void draw_filled_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c);

private:
    framebuffer& _framebuffer;
};

} // namespace renderer
