#include "application.hxx"

#include <engine3d/geometry/primitives.hxx>
#include <engine3d/renderer/opengl/opengl_renderer.hxx>
#include <engine3d/renderer/software/software_renderer.hxx>

#include <algorithm>
#include <cassert>
#include <array>
#include <cmath>
#include <memory>
#include <stdexcept>

namespace showcase {

application::application()
    : _window{{800, 600}, "Showcasse 0.1"}
    , _software_presenter{}
    , _scene{}
    , _camera{
        e3d::math::vector3{0.0F, 0.0F, 3.0F},
        e3d::math::vector3{0.0F, 0.0F, 0.0F},
        e3d::camera::perspective_projection{}
    }
    , _renderers{
        std::make_unique<e3d::renderer::software_renderer>(800, 600),
        std::make_unique<e3d::renderer::opengl::opengl_renderer>(800, 600)
    }
{
    resize_renderers();
    build_scene();
}

application::~application() = default;

int application::run()
{
    auto previous_time = clock::now();

    while( !_window.should_close() ) {
        _window.poll_events();
        resize_renderers();

        const auto current_time = clock::now();
        const duration delta_time = current_time - previous_time;
        previous_time = current_time;

        process_input(delta_time);
        update(delta_time);
        render();

        _window.swap_buffers();
    }

    return 0;
}

void application::build_scene()
{
    const auto cube = std::make_shared<const e3d::geometry::mesh>(
        e3d::geometry::primitives::cube(
            2.0F,
            std::array<e3d::graphics::colour, 8>{{
                e3d::graphics::colour{235, 70, 70},
                e3d::graphics::colour{70, 210, 120},
                e3d::graphics::colour{70, 120, 235},
                e3d::graphics::colour{245, 190, 55},
                e3d::graphics::colour{220, 80, 180},
                e3d::graphics::colour{70, 210, 220},
                e3d::graphics::colour{235, 235, 235},
                e3d::graphics::colour{145, 95, 235},
            }}));

    _scene.add(e3d::scene::scene_object{cube, e3d::scene::transform{{0.0F, 0.0F, 0.0F}, {0.45F, 0.65F, 0.12F}}});

    //const auto ground = std::make_shared<const e3d::geometry::mesh>(e3d::geometry::primitives::plane(10.0F, 10.0F));
    //_scene.add(e3d::scene::scene_object{ground, e3d::scene::transform{{0.0F, -1.0F, 0.0F}, {0.0F, 0.0F, 0.0F}}});
}

void application::resize_renderers()
{
    const auto window_size = _window.size();
    if( window_size.width == 0 || window_size.height == 0 )
        return;
    if( window_size.width == _renderer_size.width && window_size.height == _renderer_size.height )
        return;

    for( const auto& renderer : _renderers )
        renderer->resize(window_size.width, window_size.height);

    _renderer_size = window_size;
}

void application::process_input(duration delta_time)
{
    constexpr float orbit_speed = 1.5F;
    constexpr float zoom_speed = 2.0F;
    constexpr float minimum_camera_distance = 1.5F;

    if( _window.key_pressed(e3d::platform::key::escape) )
        _window.request_close();
    if( _window.key_pressed(e3d::platform::key::z) )
        _render_settings.depth_test = !_render_settings.depth_test;
    if( _window.key_pressed(e3d::platform::key::c) )
        cycle_culling();
    if( _window.key_down(e3d::platform::key::a) )
        _camera_yaw -= orbit_speed * delta_time.count();
    if( _window.key_down(e3d::platform::key::d) )
        _camera_yaw += orbit_speed * delta_time.count();
    if( _window.key_down(e3d::platform::key::w) )
        _camera_distance = std::max(minimum_camera_distance, _camera_distance - zoom_speed * delta_time.count());
    if( _window.key_down(e3d::platform::key::s) )
        _camera_distance += zoom_speed * delta_time.count();
    if( _window.key_down(e3d::platform::key::one) )
        _active_renderer_index = 0;
    if( _window.key_down(e3d::platform::key::two) )
        _active_renderer_index = 1;

    _camera.position({
        std::sin(_camera_yaw) * _camera_distance,
        0.0F,
        std::cos(_camera_yaw) * _camera_distance,
    });
    _camera.look_at({0.0F, 0.0F, 0.0F});
}

void application::cycle_culling()
{
    using enum e3d::renderer::cull_mode;

    switch( _render_settings.culling ) {
        case none:
            _render_settings.culling = back;
            break;
        case back:
            _render_settings.culling = front;
            break;
        case front:
            _render_settings.culling = none;
            break;
    }
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
    active_renderer().render(_scene, _camera, _render_settings);
    if( _active_renderer_index == 0 )
        _software_presenter.present(software_backend().framebuffer());
}

void application::switch_renderer()
{
    _active_renderer_index = (_active_renderer_index + 1)  % _renderers.size();
}

e3d::renderer::software_renderer& application::software_backend()
{
    auto* renderer = dynamic_cast<e3d::renderer::software_renderer*>(_renderers[0].get());
    if( renderer == nullptr )
        throw std::logic_error{"Software renderer is not configured at index 0."};

    return *renderer;
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
