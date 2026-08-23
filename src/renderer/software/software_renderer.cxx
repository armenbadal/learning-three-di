#include "engine3d/renderer/software/software_renderer.hxx"

namespace e3d::renderer {

software_renderer::software_renderer(std::uint32_t width, std::uint32_t height)
    : _framebuffer{width, height}
    , _pipeline{_framebuffer}
{}


void software_renderer::resize(std::uint32_t width, std::uint32_t height)
{
    _framebuffer.resize(width, height);
}


const e3d::renderer::framebuffer& software_renderer::framebuffer() const noexcept
{
    return _framebuffer;
}


void software_renderer::render(const scene::scene& world, const camera::camera& camera, const render_settings& settings)
{
    _framebuffer.clear(settings.clear_colour);
    pipeline_settings ps{
        .depth_test = settings.depth_test,
        .culling = settings.culling
    };

    for( const auto& object : world.objects() ) {
        const auto& mesh = object.mesh();

        const auto vertices = mesh.vertices();
        const auto indices = mesh.indices();

        const auto model = object.transform().matrix();

        for( std::size_t i = 0; i < indices.size(); i += 3 ) {
            const triangle3d triangle{
                vertices[indices[i]],
                vertices[indices[i + 1]],
                vertices[indices[i + 2]]
            };

            _pipeline.draw_filled_triangle(triangle, model, camera, ps);
        }
    }
}

} // namespace e3d::renderer
