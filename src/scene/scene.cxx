#include "engine3d/scene/scene.hxx"

#include <utility>

namespace e3d::scene {

scene_object& scene::add(scene_object object)
{
    _objects.push_back(std::move(object));
    return _objects.back();
}

std::span<const scene_object> scene::objects() const noexcept
{
    return _objects;
}

std::span<scene_object> scene::objects() noexcept
{
    return _objects;
}

} // namespace e3d::scene
