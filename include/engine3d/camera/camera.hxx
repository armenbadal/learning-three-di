#pragma once

#include "engine3d/math/vector3.hxx"
#include "engine3d/math/matrix4x4.hxx"

namespace e3d::camera {

class camera {
public:
    camera(const math::vector3& p, const math::vector3& t, const math::vector3& u);

    math::matrix4x4 view_matrix() const;

private:
    math::vector3 _position;
    math::vector3 _target;
    math::vector3 _up{0.0F, 1.0F, 0.0F};
};

} // namespace e3d::camera
