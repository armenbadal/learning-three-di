#pragma once

#include "engine3d/geometry/mesh.hxx"
#include "buffer.hxx"
#include "vertex_array.hxx"

namespace e3d::renderer::opengl {

class gpu_mesh final
{
public:
    explicit gpu_mesh(const geometry::mesh& mesh);

    gpu_mesh(const gpu_mesh&) = delete;
    gpu_mesh& operator=(const gpu_mesh&) = delete;

    gpu_mesh(gpu_mesh&&) noexcept = default;
    gpu_mesh& operator=(gpu_mesh&&) noexcept = default;

    void draw() const;

private:
    vertex_array _vao;
    vertex_buffer _vertices;
    index_buffer _indices;

    std::size_t _index_count{};
};

} // namespace e3d::renderer::opengl
