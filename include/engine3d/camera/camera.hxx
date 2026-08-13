#pragma once

#include "engine3d/math/transform.hxx"
#include "engine3d/math/vector3.hxx"

#include <numbers>

namespace e3d::camera {

struct camera {
    math::vector3 _position;
    math::vector3 _target;
    math::vector3 _up{0.0F, 1.0F, 0.0F};

    float _fov_y{std::numbers::pi_v<float> / 3.0F};
    float _near_plane{0.1F};
    float _far_plane{1000.0F};

    math::matrix4x4 view_matrix() const
    {
        return math::look_at(_position, _target, _up);
    }

    math::matrix4x4 projection_matrix(float aspect) const
    {
        return math::perspective(_fov_y, aspect, _near_plane, _far_plane);
    }
};

} // namespace e3d::camera
