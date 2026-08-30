#pragma once

#include "engine3d/graphics/colour.hxx"

namespace e3d::renderer {

enum class cull_mode
{
    none,
    back,
    front
};

struct render_settings {
    bool depth_test{true};
    cull_mode culling{cull_mode::back};

    graphics::colour clear_colour{13, 13, 20, 255};
};

} // namespace e3d::renderer
