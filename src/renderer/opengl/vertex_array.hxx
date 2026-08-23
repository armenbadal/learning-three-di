#pragma once

#include <glad/gl.h>

namespace e3d::renderer::opengl {

class vertex_array final {
public:
    vertex_array();
    ~vertex_array();

    vertex_array(const vertex_array&) = delete;
    vertex_array& operator=(const vertex_array&) = delete;

    vertex_array(vertex_array&& other) noexcept;
    vertex_array& operator=(vertex_array&& other) noexcept;

    void bind() const noexcept;
    static void unbind() noexcept;

private:
    GLuint _id{};
};

} // namespace e3d::renderer::opengl
