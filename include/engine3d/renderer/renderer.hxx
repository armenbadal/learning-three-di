#pragma once

#include "engine3d/scene/scene.hxx"
#include "engine3d/camera/camera.hxx"
#include "engine3d/renderer/render_settings.hxx"

#include <cstddef>
#include <string_view>

namespace e3d::renderer {

class renderer
{
public:
    renderer() = default;

    renderer(const renderer&) = delete;
    renderer& operator=(const renderer&) = delete;

    renderer(renderer&&) = delete;
    renderer& operator=(renderer&&) = delete;

    virtual ~renderer() = default;

    [[nodiscard]]
    virtual std::string_view name() const noexcept = 0;

    virtual void resize(std::uint32_t width, std::uint32_t height) = 0;

    virtual void render(const scene::scene& scene, const camera::camera& camera, const render_settings& settings) = 0;
};

} // namespace e3d::renderer
