#pragma once

#include <glad/gl.h>

#include <string_view>

namespace e3d::renderer::opengl {

class shader final {
public:
    shader(GLenum type, std::string_view source);
    ~shader();

    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    shader(shader&& other) noexcept;
    shader& operator=(shader&& other) noexcept;

    [[nodiscard]]
    GLuint id() const noexcept;

private:
    void compile(std::string_view source);

    GLuint _id{};
};

} // namespace e3d::renderer::opengl
