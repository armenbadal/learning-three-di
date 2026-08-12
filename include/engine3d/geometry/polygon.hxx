#pragma once

#include "engine3d/geometry/vertex.hxx"

#include <span>
#include <vector>

namespace engine3d::geometry {

enum class plane {
    left,
    right,
    bottom,
    top,
    near,
    far
};

std::vector<clip_vertex> clip_against_plane(std::span<const clip_vertex> input, plane pl);

} // namespace engine3d::geometry
