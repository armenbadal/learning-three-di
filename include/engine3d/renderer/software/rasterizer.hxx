#pragma once

#include "engine3d/graphics/colour.hxx"
#include "engine3d/renderer/software/framebuffer.hxx"
#include "engine3d/renderer/software/viewport.hxx"
#include "engine3d/geometry/vertex.hxx"

namespace e3d::renderer {

using geometry::vertex2d;

class rasterizer {
public:
    explicit rasterizer(framebuffer& fb);

    void draw_line(screen_vertex v0, screen_vertex v1, graphics::colour c);
    void draw_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, graphics::colour c);
    void draw_filled_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, bool depth_test = true);
    void draw_filled_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, graphics::colour c);
    void draw_filled_triangle(vertex2d v0, vertex2d v1, vertex2d v2);

private:
    void rasterize_filled_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, const graphics::colour* fill_colour, bool depth_test);

    framebuffer& _framebuffer;
};

} // namespace e3d::renderer
