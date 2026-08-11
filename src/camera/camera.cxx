#include "engine3d/camera/camera.hxx"
#include "engine3d/math/transform.hxx"

namespace engine3d::renderer {

camera::camera(const math::vector3& p, const math::vector3& t, const math::vector3& u)
    : _position{p}, _target{t}, _up{u}
{}


math::matrix4x4 camera::view_matrix() const
{
    return math::look_at(_position, _target, _up);
}

} // namespace engine3d::renderer
