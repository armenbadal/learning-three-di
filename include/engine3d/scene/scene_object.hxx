#pragma once

#include "transform.hxx"
#include "engine3d/geometry/mesh.hxx"

#include <memory>

namespace e3d {

class scene_object final {
public:
    explicit scene_object(std::shared_ptr<const geometry::mesh> mesh, transform transform = {});

    [[nodiscard]]
    const geometry::mesh& mesh() const noexcept;

    [[nodiscard]]
    const e3d::transform& transform() const noexcept;

    [[nodiscard]]
    e3d::transform& transform() noexcept;

private:
    std::shared_ptr<const geometry::mesh> _mesh;
    e3d::transform _transform;
};

} // namespace e3d
