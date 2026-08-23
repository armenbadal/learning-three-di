#include "engine3d/geometry/mesh.hxx"

#include <utility>

namespace e3d::geometry {

mesh::mesh(std::vector<vertex> vertices, std::vector<std::uint32_t> indices)
    : _vertices{std::move(vertices)}
    , _indices{std::move(indices)}
{}

std::span<const vertex> mesh::vertices() const noexcept
{
    return _vertices;
}

std::span<const std::uint32_t> mesh::indices() const noexcept
{
    return _indices;
}

std::size_t mesh::triangle_count() const noexcept
{
    return _indices.size() / 3;
}

} // namespace e3d::geometry
