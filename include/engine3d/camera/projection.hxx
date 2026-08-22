#pragma once

#include <numbers>
#include <variant>

namespace e3d::camera {

struct perspective_projection {
    float fov_y{std::numbers::pi_v<float> / 3.0F};
    float near_plane{0.1F};
    float far_plane{1000.0F};
};

struct orthographic_projection {
    float height{10.0F};
    float near_plane{0.1F};
    float far_plane{1000.0F};
};

using projection = std::variant<perspective_projection, orthographic_projection>;

} // namespace e3d::camera
