#pragma once

#include "engine3d/renderer/renderer.hxx"

#include <memory>

namespace e3d::renderer::opengl {

class opengl_renderer final : public renderer {
public:
    opengl_renderer();
    ~opengl_renderer() override = default;

    [[nodiscard]]
    std::string_view name() const noexcept override
    {
        return "OpenGL";
    }

    void resize(std::uint32_t width, std::uint32_t height) override;

    void render(const scene::scene& scene, const camera::camera& camera, const render_settings& settings) override;

private:
    class implementation;
    std::unique_ptr<implementation> _impl;
};

} // namespace e3d::renderer::opengl
