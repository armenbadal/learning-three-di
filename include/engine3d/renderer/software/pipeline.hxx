#pragma once

#include "engine3d/camera/camera.hxx"
#include "engine3d/geometry/vertex.hxx"
#include "engine3d/graphics/colour.hxx"
#include "engine3d/math/matrix4x4.hxx"
#include "engine3d/math/vector3.hxx"
#include "engine3d/renderer/software/framebuffer.hxx"
#include "engine3d/renderer/software/rasterizer.hxx"
#include "engine3d/renderer/software/viewport.hxx"
#include "engine3d/renderer/render_settings.hxx"

#include <array>
#include <vector>

namespace e3d::renderer {

using triangle3d = std::array<geometry::vertex, 3>;
using screen_triangle = std::array<screen_vertex, 3>;

struct pipeline_settings {
    bool depth_test{true};
    cull_mode culling{cull_mode::none};
};

class pipeline {
public:
    explicit pipeline(framebuffer& fb);

    std::vector<screen_triangle> transform_triangle(const triangle3d& triangle, const math::matrix4x4& model, const camera::camera& camera) const;
    void draw_filled_triangle(const triangle3d& triangle, const math::matrix4x4& model, const camera::camera& camera, graphics::colour colour);
    void draw_filled_triangle(const triangle3d& triangle, const math::matrix4x4& model, const camera::camera& camera, const pipeline_settings& settings = {});

private:
    framebuffer& _framebuffer;
    rasterizer _rasterizer;
};

} // namespace e3d::renderer
