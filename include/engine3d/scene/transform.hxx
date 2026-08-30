#pragma once

#include "engine3d/math/matrix4x4.hxx"
#include "engine3d/math/vector3.hxx"

namespace e3d::scene {

class transform final {
public:
    transform() noexcept = default;

    transform(math::vector3 position, math::vector3 rotation, math::vector3 scale = {1.0F, 1.0F, 1.0F}) noexcept;

    [[nodiscard]]
    const math::vector3& position() const noexcept;

    [[nodiscard]]
    const math::vector3& rotation() const noexcept;

    [[nodiscard]]
    const math::vector3& scale() const noexcept;

    void position(math::vector3 value) noexcept;
    void rotation(math::vector3 value) noexcept;
    void scale(math::vector3 value) noexcept;

    [[nodiscard]]
    math::matrix4x4 matrix() const noexcept;

private:
    math::vector3 _position{};
    math::vector3 _rotation{};
    math::vector3 _scale{1.0F, 1.0F, 1.0F};
};

} // namespace e3d::scene
