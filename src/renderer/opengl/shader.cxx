#include "shader.hxx"

#include <stdexcept>
#include <string>
#include <utility>

namespace e3d::renderer::opengl {

shader::shader(GLenum type, std::string_view source)
    : _id{glCreateShader(type)}
{
    if( _id == 0 )
        throw std::runtime_error{"Failed to create OpenGL shader."};

    compile(source);
}

void shader::compile(std::string_view source)
{
    const char* source_data = source.data();
    const GLint source_length = static_cast<GLint>(source.size());
    glShaderSource(_id, 1, &source_data, &source_length);
    glCompileShader(_id);

    GLint compiled = GL_FALSE;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &compiled);
    if( compiled == GL_TRUE )
        return;

    GLint log_length = 0;
    glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &log_length);
    std::string log(static_cast<std::size_t>(log_length), '\0');
    glGetShaderInfoLog(_id, log_length, nullptr, log.data());

    glDeleteShader(_id);
    _id = 0;
    throw std::runtime_error{"Failed to compile OpenGL shader: " + log};
}

shader::~shader()
{
    if( _id != 0 )
        glDeleteShader(_id);
}

shader::shader(shader&& other) noexcept
    : _id{std::exchange(other._id, 0)}
{}

shader& shader::operator=(shader&& other) noexcept
{
    if( this == &other )
        return *this;

    if( _id != 0 )
        glDeleteShader(_id);
    _id = std::exchange(other._id, 0);
    return *this;
}

GLuint shader::id() const noexcept
{
    return _id;
}

} // namespace e3d::renderer::opengl
