#pragma once

#include "vertex.hxx"

#include <cstddef>
#include <span>
#include <vector>

namespace e3d::geometry {

class mesh final {
public:
    mesh(std::vector<vertex> vertices, std::vector<std::uint32_t> indices);

    [[nodiscard]]
    std::span<const vertex> vertices() const noexcept;

    [[nodiscard]]
    std::span<const index_type> indices() const noexcept;

    [[nodiscard]]
    std::size_t triangle_count() const noexcept;

private:
    std::vector<vertex> _vertices;
    std::vector<std::uint32_t> _indices;
};

} // namespace e3d::geometry
