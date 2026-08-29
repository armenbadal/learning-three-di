#include "application.hxx"

#include <engine3d/geometry/mesh.hxx>
#include <engine3d/renderer/opengl/opengl_renderer.hxx>
#include <engine3d/renderer/software/software_renderer.hxx>

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

namespace showcase {

application::application()
    : _window{{800, 600}, "Showcasse 0.1"}
    , _software_renderer{800, 600}
    , _software_presenter{}
    , _scene{}
    , _camera{
        e3d::math::vector3{0.0F, 0.0F, 3.0F},
        e3d::math::vector3{0.0F, 0.0F, 0.0F},
        e3d::camera::perspective_projection{}}
    , _render_settings{}
    // , _renderers{
    //       std::make_unique<
    //           engine3d::renderer::software_renderer>(
    //           /* ... */),

    //       std::make_unique<
    //           engine3d::renderer::opengl::opengl_renderer>(
    //           /* ... */)}
{
    build_scene();
}

application::~application() = default;

int application::run()
{
    auto previous_time = clock::now();

    while( !_window.should_close() ) {
        _window.poll_events();
        resize_software_renderer();

        const auto current_time = clock::now();
        const duration delta_time = current_time - previous_time;
        previous_time = current_time;

        process_input();
        update(delta_time);
        render();

        _window.swap_buffers();
    }

    return 0;
}

void application::build_scene()
{
    const auto cube = std::make_shared<const e3d::geometry::mesh>(
        std::vector<e3d::geometry::vertex>{
            {{-1.0F, -1.0F, -1.0F}, {235, 70, 70}},
            {{1.0F, -1.0F, -1.0F}, {70, 210, 120}},
            {{1.0F, 1.0F, -1.0F}, {70, 120, 235}},
            {{-1.0F, 1.0F, -1.0F}, {245, 190, 55}},
            {{-1.0F, -1.0F, 1.0F}, {220, 80, 180}},
            {{1.0F, -1.0F, 1.0F}, {70, 210, 220}},
            {{1.0F, 1.0F, 1.0F}, {235, 235, 235}},
            {{-1.0F, 1.0F, 1.0F}, {145, 95, 235}},
        },
        std::vector<std::uint32_t>{
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            0, 4, 5, 0, 5, 1,
            3, 2, 6, 3, 6, 7,
            1, 5, 6, 1, 6, 2,
            0, 3, 7, 0, 7, 4,
        });

    _scene.add(e3d::scene::scene_object{
        cube,
        e3d::scene::transform{{0.0F, 0.0F, 0.0F}, {0.45F, 0.65F, 0.12F}}});
}

void application::resize_software_renderer()
{
    const auto window_size = _window.size();
    if( window_size.width == 0 || window_size.height == 0 )
        return;

    const auto& framebuffer = _software_renderer.framebuffer();
    if( framebuffer.width() == window_size.width && framebuffer.height() == window_size.height )
        return;

    _software_renderer.resize(window_size.width, window_size.height);
}

void application::process_input()
{
}

void application::update(duration delta_time)
{
    constexpr float angular_speed = 0.7F;

    auto objects = _scene.objects();

    if( objects.empty() )
        return;

    auto& cube = objects.front();
    auto rotation = cube.transform().rotation();
    rotation = decltype(rotation)(rotation.x(), rotation.y() + angular_speed * delta_time.count(), rotation.z());
    cube.transform().rotation(rotation);
}

void application::render()
{
    _software_renderer.render(_scene, _camera, _render_settings);
    _software_presenter.present(_software_renderer.framebuffer());
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
