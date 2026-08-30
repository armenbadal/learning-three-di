#pragma once

#include "software_presenter.hxx"

#include <array>
#include <chrono>
#include <cstddef>
#include <memory>

#include <engine3d/camera/camera.hxx>
#include <engine3d/platform/window.hxx>
#include <engine3d/renderer/render_settings.hxx>
#include <engine3d/renderer/renderer.hxx>
#include <engine3d/renderer/software/software_renderer.hxx>
#include <engine3d/renderer/opengl/opengl_renderer.hxx>
#include <engine3d/scene/scene.hxx>

namespace showcase {

class application final {
public:
    application();

    application(const application&) = delete;
    application& operator=(const application&) = delete;

    application(application&&) = delete;
    application& operator=(application&&) = delete;

    ~application();

    int run();

private:
    using clock = std::chrono::steady_clock;
    using duration = std::chrono::duration<float>;
    using renderer_ptr = std::unique_ptr<e3d::renderer::renderer>;

    static constexpr std::size_t renderer_count = 2;

    void build_scene();
    void resize_software_renderer();
    void cycle_culling();
    void process_input(duration delta_time);
    void update(duration delta_time);
    void render();

    void switch_renderer();

    [[nodiscard]]
    e3d::renderer::renderer& active_renderer() noexcept;
    [[nodiscard]]
    const e3d::renderer::renderer& active_renderer() const noexcept;

private:
    e3d::platform::window _window;
    e3d::renderer::software_renderer _software_renderer;
    software_presenter _software_presenter;
    e3d::renderer::opengl::opengl_renderer _opengl_renderer;

    e3d::scene::scene _scene;
    e3d::camera::camera _camera;

    e3d::renderer::render_settings _render_settings{};

    std::array<renderer_ptr, renderer_count> _renderers;
    std::size_t _active_renderer_index{};

    float _camera_yaw{0.0F};
    float _camera_distance{3.0F};
};

} // namespace showcase
