#pragma once

#include "engine3d/math/vector2.hxx"
#include "engine3d/math/vector3.hxx"
#include "engine3d/graphics/colour.hxx"
#include "engine3d/renderer/framebuffer.hxx"
#include "engine3d/renderer/viewport.hxx"
#include "engine3d/geometry/vertex.hxx"

#include <tuple>
#include <vector>

namespace e3d::renderer {

using math::vector2;
using math::vector3;
using geometry::vertex2d;

class rasterizer {
public:
    explicit rasterizer(framebuffer& fb);

    void draw_line(screen_vertex v0, screen_vertex v1, graphics::colour c);
    void draw_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, graphics::colour c);
    void draw_filled_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, graphics::colour c);
    void draw_filled_triangle(vertex2d v0, vertex2d v1, vertex2d v2);

private:
    framebuffer& _framebuffer;
    std::vector<float> _depth_buffer;
};

} // namespace e3d::renderer
