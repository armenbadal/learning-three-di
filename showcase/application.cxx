#include "application.hxx"

#include <engine3d/renderer/opengl/opengl_renderer.hxx>
#include <engine3d/renderer/software/software_renderer.hxx>

#include <cassert>
#include <memory>

namespace showcase {

application::application()
    : _camera{
        e3d::math::vector3{0.0F, 0.0F, 3.0F},
        e3d::math::vector3{0.0F, 0.0F, 0.0F},
        e3d::camera::perspective_projection{}}
    // , _window{/* width, height, title */}
    // , _scene{}
    // , _camera{/* ... */}
    // , _render_settings{}
    // , _renderers{
    //       std::make_unique<
    //           engine3d::renderer::software_renderer>(
    //           /* ... */),

    //       std::make_unique<
    //           engine3d::renderer::opengl::opengl_renderer>(
    //           /* ... */)}
{
}

application::~application() = default;

int application::run()
{
    auto previous_time = clock::now();

    while( !_window.should_close() ) {
        const auto current_time = clock::now();
        const duration delta_time = current_time - previous_time;
        previous_time = current_time;

        process_input();
        update(delta_time);
        render();

        _window.swap_buffers();
        _window.poll_events();
    }

    return 0;
}

void application::process_input()
{
}

void application::update(duration delta_time)
{
    (void)delta_time;
}

void application::render()
{
    active_renderer().render(_scene, _camera, _render_settings);
}

void application::switch_renderer()
{
    _active_renderer_index = (_active_renderer_index + 1)  % _renderers.size();
}

e3d::renderer::renderer& application::active_renderer() noexcept
{
    assert(_renderers[_active_renderer_index]);
    return *_renderers[_active_renderer_index];
}

const e3d::renderer::renderer& application::active_renderer() const noexcept
{
    assert(_renderers[_active_renderer_index]);
    return *_renderers[_active_renderer_index];
}

} // namespace showcase
