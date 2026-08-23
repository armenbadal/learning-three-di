#pragma once

#include "engine3d/geometry/vertex.hxx"

#include <glad/gl.h>

#include <cstdint>
#include <span>

namespace e3d::renderer::opengl {

class vertex_buffer final {
public:
    vertex_buffer();
    explicit vertex_buffer(std::span<const geometry::vertex> vertices);
    ~vertex_buffer();

    vertex_buffer(const vertex_buffer&) = delete;
    vertex_buffer& operator=(const vertex_buffer&) = delete;

    vertex_buffer(vertex_buffer&& other) noexcept;
    vertex_buffer& operator=(vertex_buffer&& other) noexcept;

    void bind() const noexcept;
    void upload(std::span<const geometry::vertex> vertices) const noexcept;
    static void unbind() noexcept;

private:
    GLuint _id{};
};

class index_buffer final {
public:
    index_buffer();
    explicit index_buffer(std::span<const std::uint32_t> indices);
    ~index_buffer();

    index_buffer(const index_buffer&) = delete;
    index_buffer& operator=(const index_buffer&) = delete;

    index_buffer(index_buffer&& other) noexcept;
    index_buffer& operator=(index_buffer&& other) noexcept;

    void bind() const noexcept;
    void upload(std::span<const std::uint32_t> indices) const noexcept;
    static void unbind() noexcept;

private:
    GLuint _id{};
};

} // namespace e3d::renderer::opengl
