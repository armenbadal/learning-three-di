#pragma once

#include "engine3d/graphics/colour.hxx"

#include <cstddef>
#include <span>
#include <vector>

namespace e3d::renderer {

class framebuffer {
public:
    framebuffer(std::size_t width, std::size_t height);

    std::size_t width() const noexcept { return _width; }
    std::size_t height() const noexcept { return _height; }

    // Replaces both buffers with cleared storage for the requested dimensions.
    // Throws std::invalid_argument when the dimensions cannot be represented.
    void resize(std::size_t width, std::size_t height);

    // Checked access; throws std::out_of_range for coordinates outside the buffer.
    graphics::colour& at(std::size_t x, std::size_t y);
    const graphics::colour& at(std::size_t x, std::size_t y) const;
    float& depth_at(std::size_t x, std::size_t y);
    const float& depth_at(std::size_t x, std::size_t y) const;

    // Unchecked access; caller must keep coordinates in bounds.
    graphics::colour& operator()(std::size_t x, std::size_t y) noexcept;
    const graphics::colour& operator()(std::size_t x, std::size_t y) const noexcept;

    std::span<graphics::colour> pixels() noexcept { return _pixels; }
    std::span<const graphics::colour> pixels() const noexcept { return _pixels; }
    std::span<float> depths() noexcept { return _depth_buffer; }
    std::span<const float> depths() const noexcept { return _depth_buffer; }

    // Clipped write; out-of-bounds coordinates are silently ignored.
    void set_clipped(int x, int y, graphics::colour p) noexcept;

    void clear_colour(graphics::colour c = graphics::black) noexcept;
    void clear_depth() noexcept;
    void clear(graphics::colour c = graphics::black) noexcept;

private:
    std::size_t checked_index(std::size_t x, std::size_t y) const;

    std::size_t _width;
    std::size_t _height;
    std::vector<graphics::colour> _pixels;
    std::vector<float> _depth_buffer;
};

} // namespace e3d::renderer
