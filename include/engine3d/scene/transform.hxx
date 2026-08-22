#pragma once

#include "engine3d/math/matrix4x4.hxx"
#include "engine3d/math/vector3.hxx"

namespace e3d {

class transform final {
public:
    constexpr transform() noexcept = default;

    constexpr transform(math::vector3 position, math::vector3 rotation, math::vector3 scale = {1.0F, 1.0F, 1.0F}) noexcept;

    [[nodiscard]]
    constexpr const math::vector3& position() const noexcept;

    [[nodiscard]]
    constexpr const math::vector3& rotation() const noexcept;

    [[nodiscard]]
    constexpr const math::vector3& scale() const noexcept;

    constexpr void position(math::vector3 value) noexcept;
    constexpr void rotation(math::vector3 value) noexcept;
    constexpr void scale(math::vector3 value) noexcept;

    [[nodiscard]]
    math::matrix4x4 matrix() const noexcept;

private:
    math::vector3 _position{};
    math::vector3 _rotation{};
    math::vector3 _scale{1.0F, 1.0F, 1.0F};
};

} // namespace e3d
