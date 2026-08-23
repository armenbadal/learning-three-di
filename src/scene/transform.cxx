#include "engine3d/scene/transform.hxx"

#include "engine3d/math/transform.hxx"

#include <utility>

namespace e3d::scene {

transform::transform(math::vector3 position, math::vector3 rotation, math::vector3 scale) noexcept
    : _position{std::move(position)}
    , _rotation{std::move(rotation)}
    , _scale{std::move(scale)}
{}

const math::vector3& transform::position() const noexcept
{
    return _position;
}

const math::vector3& transform::rotation() const noexcept
{
    return _rotation;
}

const math::vector3& transform::scale() const noexcept
{
    return _scale;
}

void transform::position(math::vector3 value) noexcept
{
    _position = std::move(value);
}

void transform::rotation(math::vector3 value) noexcept
{
    _rotation = std::move(value);
}

void transform::scale(math::vector3 value) noexcept
{
    _scale = std::move(value);
}

math::matrix4x4 transform::matrix() const noexcept
{
    return math::model_matrix(_position, _rotation, _scale);
}

} // namespace e3d::scene
