#pragma once

#include "engine3d/math/vector4.hxx"

#include <span>
#include <vector>

namespace e3d::renderer {

struct clip_vertex {
    e3d::math::vector4 position;
};

enum class clip_plane {
    left,
    right,
    bottom,
    top,
    near,
    far
};

std::vector<clip_vertex> clip_against_plane(std::span<const clip_vertex> input, clip_plane pl);
std::vector<clip_vertex> clip_polygon(std::span<const clip_vertex> input);

} // namespace e3d::renderer
