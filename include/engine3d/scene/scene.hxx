#pragma once

#include "engine3d/scene/scene_object.hxx"

#include <span>
#include <vector>

namespace e3d::scene {

class scene final {
public:
    scene() = default;

    scene_object& add(scene_object object);

    [[nodiscard]]
    std::span<const scene_object> objects() const noexcept;

    [[nodiscard]]
    std::span<scene_object> objects() noexcept;

private:
    std::vector<scene_object> _objects;
};

} // namespace e3d::scene
