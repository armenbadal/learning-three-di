#include "engine3d/camera/camera.hxx"
#include "engine3d/math/transform.hxx"

#include <cassert>
#include <utility>

namespace {

template <typename... functions>
struct overloaded : functions... {
    using functions::operator()...;
};

template <typename... functions>
overloaded(functions...) -> overloaded<functions...>;

} // namespace

namespace e3d::camera {

camera::camera(math::vector3 position, math::vector3 target, e3d::camera::projection projection)
    : _position{position}, _target{target}, _projection{std::move(projection)}
{}

math::matrix4x4 camera::view_matrix() const
{
    return math::look_at(_position, _target, _up);
}

math::matrix4x4 camera::projection_matrix(float aspect) const
{
    return std::visit(
        overloaded{
            [aspect](const perspective_projection& projection) {
                return math::perspective(projection.fov_y, aspect, projection.near_plane, projection.far_plane);
            },
            [aspect](const orthographic_projection& projection) -> math::matrix4x4 {
                assert(aspect > 0.0F);
                assert(projection.height > 0.0F);
                assert(projection.near_plane > 0.0F);
                assert(projection.far_plane > projection.near_plane);

                const float width = projection.height * aspect;
                const float depth = projection.far_plane - projection.near_plane;
                return {
                    2.0F / width, 0.0F, 0.0F, 0.0F,
                    0.0F, 2.0F / projection.height, 0.0F, 0.0F,
                    0.0F, 0.0F, -2.0F / depth,
                    -(projection.far_plane + projection.near_plane) / depth,
                    0.0F, 0.0F, 0.0F, 1.0F,
                };
            },
        },
        _projection);
}

const math::vector3& camera::position() const noexcept
{
    return _position;
}

void camera::position(math::vector3 value) noexcept
{
    _position = value;
}

void camera::look_at(math::vector3 target) noexcept
{
    _target = target;
}

const e3d::camera::projection& camera::projection() const noexcept
{
    return _projection;
}

void camera::projection(e3d::camera::projection value)
{
    _projection = std::move(value);
}

} // namespace e3d::camera
