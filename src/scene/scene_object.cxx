#include "engine3d/scene/scene_object.hxx"

#include <stdexcept>
#include <utility>

namespace e3d::scene {

scene_object::scene_object(std::shared_ptr<const geometry::mesh> mesh, e3d::scene::transform transform)
    : _mesh{std::move(mesh)}
    , _transform{std::move(transform)}
{
    if( !_mesh )
        throw std::invalid_argument{"scene object requires a mesh"};
}

const geometry::mesh& scene_object::mesh() const noexcept
{
    return *_mesh;
}

const e3d::scene::transform& scene_object::transform() const noexcept
{
    return _transform;
}

e3d::scene::transform& scene_object::transform() noexcept
{
    return _transform;
}

} // namespace e3d::scene
