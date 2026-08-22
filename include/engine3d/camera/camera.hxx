#pragma once

#include "engine3d/math/transform.hxx"
#include "engine3d/math/vector3.hxx"
#include "engine3d/camera/projection.hxx"

#include <numbers>

namespace e3d::camera {

class camera final {
public:
    camera(math::vector3 position, math::vector3 target, projection projection);

    [[nodiscard]]
    math::matrix4x4 view_matrix() const;

    [[nodiscard]]
    math::matrix4x4 projection_matrix(float aspect) const;

    [[nodiscard]]
    const math::vector3& position() const noexcept;

    void position(math::vector3 value) noexcept;
    void look_at(math::vector3 target) noexcept;

    [[nodiscard]]
    const projection& projection() const noexcept;

    void projection(e3d::camera::projection value);

private:
    math::vector3 _position{};
    math::vector3 _target{};
    math::vector3 _up{0.0F, 1.0F, 0.0F};

    e3d::camera::projection _projection;
};

} // namespace e3d::camera
