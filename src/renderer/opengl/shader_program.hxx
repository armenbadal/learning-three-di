#pragma once

#include "engine3d/math/matrix4x4.hxx"
#include "engine3d/math/vector3.hxx"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <string_view>

namespace e3d::renderer::opengl {

class shader_program final {
public:
    shader_program(std::string_view vertex_source, std::string_view fragment_source);
    ~shader_program();

    shader_program(const shader_program&) = delete;
    shader_program& operator=(const shader_program&) = delete;

    shader_program(shader_program&&) noexcept;
    shader_program& operator=(shader_program&&) noexcept;

    void use() const noexcept;

    void set_uniform(std::string_view name, const math::matrix4x4& value);
    void set_uniform(std::string_view name, const math::vector3& value);
    void set_uniform(std::string_view name, float value);

private:
    GLuint _id{};
};

} // namespace e3d::renderer::opengl 
