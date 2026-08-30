#pragma once

#include "engine3d/renderer/renderer.hxx"
#include "engine3d/renderer/software/framebuffer.hxx"
#include "engine3d/renderer/software/pipeline.hxx"

#include <cstddef>
#include <string_view>

namespace e3d::renderer {

class software_renderer final : public renderer {
public:
    software_renderer(std::uint32_t width, std::uint32_t height);
    ~software_renderer() override = default;

    [[nodiscard]]
    std::string_view name() const noexcept override
    {
        return "Software";
    }

    void resize(std::uint32_t width, std::uint32_t height) override;

    void render(const scene::scene& scene, const camera::camera& camera, const render_settings& settings) override;

    [[nodiscard]]
    const e3d::renderer::framebuffer& framebuffer() const noexcept;

private:
    e3d::renderer::framebuffer _framebuffer;
    pipeline _pipeline;
};

} // namespace e3d::renderer
