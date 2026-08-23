#include "shader_program.hxx"

#include "shader.hxx"

#include <stdexcept>
#include <string>
#include <utility>

namespace e3d::renderer::opengl {

shader_program::shader_program(std::string_view vertex_source, std::string_view fragment_source)
{
    create_and_link(vertex_source, fragment_source);
}

void shader_program::create_and_link(std::string_view vertex_source, std::string_view fragment_source)
{
    const GLuint program = glCreateProgram();
    if( program == 0 )
        throw std::runtime_error{"Failed to create OpenGL shader program."};

    try {
        shader vertex_shader{GL_VERTEX_SHADER, vertex_source};
        shader fragment_shader{GL_FRAGMENT_SHADER, fragment_source};
        glAttachShader(program, vertex_shader.id());
        glAttachShader(program, fragment_shader.id());
        glLinkProgram(program);

        GLint linked = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if( linked == GL_TRUE ) {
            _id = program;
            return;
        }

        GLint log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        std::string log(static_cast<std::size_t>(log_length), '\0');
        glGetProgramInfoLog(program, log_length, nullptr, log.data());
        throw std::runtime_error{"Failed to link OpenGL shader program: " + log};
    }
    catch( ... ) {
        glDeleteProgram(program);
        throw;
    }
}

shader_program::~shader_program()
{
    if( _id != 0 )
        glDeleteProgram(_id);
}

shader_program::shader_program(shader_program&& other) noexcept
    : _id{std::exchange(other._id, 0)}
{}

shader_program& shader_program::operator=(shader_program&& other) noexcept
{
    if( this == &other )
        return *this;

    if( _id != 0 )
        glDeleteProgram(_id);
    _id = std::exchange(other._id, 0);
    return *this;
}

void shader_program::use() const noexcept
{
    glUseProgram(_id);
}

void shader_program::set_uniform(std::string_view name, const math::matrix4x4& value)
{
    glUniformMatrix4fv(uniform_location(name), 1, GL_TRUE, value.data());
}

void shader_program::set_uniform(std::string_view name, const math::vector3& value)
{
    glUniform3f(uniform_location(name), value.x(), value.y(), value.z());
}

void shader_program::set_uniform(std::string_view name, float value)
{
    glUniform1f(uniform_location(name), value);
}

GLint shader_program::uniform_location(std::string_view name) const
{
    const std::string null_terminated_name{name};
    return glGetUniformLocation(_id, null_terminated_name.c_str());
}

} // namespace e3d::renderer::opengl
