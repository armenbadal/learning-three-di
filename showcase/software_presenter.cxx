#include "software_presenter.hxx"

#include <engine3d/renderer/software/framebuffer.hxx>

#include <glad/gl.h>

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

namespace showcase {

namespace {

constexpr std::string_view vertex_shader_source = R"(
#version 460 core

out vec2 v_uv;

const vec2 positions[3] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 3.0, -1.0),
    vec2(-1.0,  3.0)
);

void main()
{
    vec2 position = positions[gl_VertexID];

    gl_Position = vec4(position, 0.0, 1.0);
    vec2 uv = position * 0.5 + 0.5;
    v_uv = vec2(uv.x, 1.0 - uv.y);
}
)";

constexpr std::string_view fragment_shader_source = R"(
#version 460 core

in vec2 v_uv;

uniform sampler2D u_framebuffer;

out vec4 out_colour;

void main()
{
    out_colour = texture(u_framebuffer, v_uv);
}
)";

GLuint compile_shader(GLenum type, std::string_view source)
{
    const GLuint shader = glCreateShader(type);
    if( shader == 0 )
        throw std::runtime_error{"Failed to create OpenGL shader."};

    const char* source_data = source.data();
    const GLint source_length = static_cast<GLint>(source.size());
    glShaderSource(shader, 1, &source_data, &source_length);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if( compiled == GL_TRUE )
        return shader;

    GLint log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    std::string log(static_cast<std::size_t>(log_length), '\0');
    glGetShaderInfoLog(shader, log_length, nullptr, log.data());
    glDeleteShader(shader);
    throw std::runtime_error{"Failed to compile OpenGL shader: " + log};
}

GLuint create_shader_program()
{
    const GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    const GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    const GLuint program = glCreateProgram();
    if( program == 0 ) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        throw std::runtime_error{"Failed to create OpenGL shader program."};
    }

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if( linked == GL_TRUE )
        return program;

    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    std::string log(static_cast<std::size_t>(log_length), '\0');
    glGetProgramInfoLog(program, log_length, nullptr, log.data());
    glDeleteProgram(program);
    throw std::runtime_error{"Failed to link OpenGL shader program: " + log};
}

} // namespace


class software_presenter::implementation final {
public:
    implementation() {}

    ~implementation()
    {
        if( _texture != 0 )
            glDeleteTextures(1, &_texture);

        if( _vao != 0 )
            glDeleteVertexArrays(1, &_vao);

        if( _program != 0 )
            glDeleteProgram(_program);
    }

    void present(const e3d::renderer::framebuffer& framebuffer)
    {
        if( framebuffer.width() == 0 || framebuffer.height() == 0)
            return;

        ensure_texture(framebuffer.width(), framebuffer.height());
        upload(framebuffer);
        glViewport(0, 0, static_cast<GLsizei>(framebuffer.width()), static_cast<GLsizei>(framebuffer.height()));
        draw();
    }

private:
    void create_program()
    {
        _program = create_shader_program();
    }

    void create_vertex_array()
    {
        glCreateVertexArrays(1, &_vao);
    }

    void ensure_texture(std::size_t width, std::size_t height)
    {
        if( _texture != 0 && width == _texture_width && height == _texture_height )
            return;

        if( _texture != 0 ) {
            glDeleteTextures(1, &_texture);
            _texture = 0;
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &_texture);
        glTextureStorage2D(_texture, 1, GL_RGBA8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
        glTextureParameteri(_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        _texture_width = width;
        _texture_height = height;
    }

    void upload(const e3d::renderer::framebuffer& framebuffer)
    {
        const auto pixels = framebuffer.pixels();

        glTextureSubImage2D(_texture, 0, 0, 0,
            static_cast<GLsizei>(framebuffer.width()),
            static_cast<GLsizei>(framebuffer.height()),
            GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    }

    void draw()
    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glUseProgram(_program);
        glBindTextureUnit(0, _texture);
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0,3);
    }

private:
    GLuint _texture{};
    GLuint _program{};
    GLuint _vao{};

    std::size_t _texture_width{};
    std::size_t _texture_height{};
};


software_presenter::software_presenter()
    : _impl{std::make_unique<implementation>()}
{}

software_presenter::~software_presenter() = default;

void software_presenter::present(const e3d::renderer::framebuffer& framebuffer)
{
    _impl->present(framebuffer);
}

} // namespace showcase
