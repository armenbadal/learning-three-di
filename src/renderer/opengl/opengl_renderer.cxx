#include "engine3d/renderer/opengl/opengl_renderer.hxx"

#include "gpu_mesh.hxx"
#include "shader_program.hxx"

#include <unordered_map>

namespace {

constexpr std::string_view vertex_shader_source = R"(
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec4 a_colour;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 v_colour;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);

    v_colour = a_colour;
}
)";

constexpr std::string_view fragment_shader_source = R"(
#version 460 core

in vec4 v_colour;

out vec4 out_colour;

void main()
{
    out_colour = v_colour;
}
)";

} // namespace

namespace e3d::renderer::opengl {

class opengl_renderer::implementation final {
public:
    implementation()
    {
        glFrontFace(GL_CCW);
    }

    void resize(std::uint32_t width, std::uint32_t height)
    {
        _width = width;
        _height = height;
    }

    void render(const scene::scene& scene, const camera::camera& camera, const render_settings& settings)
    {
        if( _width == 0 || _height == 0 )
            return;

        glViewport(0, 0, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height));
        apply_render_settings(settings);

        auto f = [](std::uint8_t value) { return static_cast<float>(value) / 255.0F; };

        const auto& c = settings.clear_colour;
        glClearColor(f(c.r()), f(c.g()), f(c.b()), f(c.a()));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _program.use();

        const auto view = camera.view_matrix();
        _program.set_uniform("u_view", view);

        const float aspect = static_cast<float>(_width) / static_cast<float>(_height);
        const auto projection = camera.projection_matrix(aspect);
        _program.set_uniform("u_projection", projection);

        for( const auto& object : scene.objects() ) {
            _program.set_uniform("u_model", object.transform().matrix());
            mesh_for(object.mesh()).draw();
        }
    }

private:
    void apply_render_settings(const render_settings& settings)
    {
        if( settings.depth_test )
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);

        switch( settings.culling ) {
            case cull_mode::none:
                glDisable(GL_CULL_FACE);
                break;
            case cull_mode::back:
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                break;
            case cull_mode::front:
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                break;
        }
    }

    gpu_mesh& mesh_for(const geometry::mesh& mesh)
    {
        auto [it, inserted] = _meshes.try_emplace(&mesh, mesh);
        return it->second;
    }

private:
    shader_program _program {vertex_shader_source, fragment_shader_source };

    std::unordered_map<const geometry::mesh*, gpu_mesh> _meshes;

    std::uint32_t _width{};
    std::uint32_t _height{};
};


opengl_renderer::opengl_renderer()
    : _impl{std::make_unique<implementation>()}
{}

opengl_renderer::~opengl_renderer() = default;


void opengl_renderer::resize(std::uint32_t width, std::uint32_t height)
{
    _impl->resize(width, height);
}

void opengl_renderer::render(const scene::scene& scene, const camera::camera& camera, const render_settings& settings)
{
    _impl->render(scene, camera, settings);
}

} // namespace e3d::renderer::opengl
