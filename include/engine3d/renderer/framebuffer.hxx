#pragma once

#include "engine3d/renderer/colour.hxx"

#include <cstddef>
#include <span>
#include <vector>

namespace engine3d::renderer {

class framebuffer {
public:
    framebuffer(std::size_t width, std::size_t height);

    std::size_t width() const noexcept { return _width; }
    std::size_t height() const noexcept { return _height; }

    // Checked access; throws std::out_of_range for coordinates outside the buffer.
    colour& at(std::size_t x, std::size_t y);
    const colour& at(std::size_t x, std::size_t y) const;

    // Unchecked access; caller must keep coordinates in bounds.
    colour& operator()(std::size_t x, std::size_t y) noexcept;
    const colour& operator()(std::size_t x, std::size_t y) const noexcept;

    std::span<colour> pixels() noexcept { return _pixels; }
    std::span<const colour> pixels() const noexcept { return _pixels; }

    // Clipped write; out-of-bounds coordinates are silently ignored.
    void set_clipped(int x, int y, colour p) noexcept;

    void clear(colour c = black) noexcept;

private:
    std::size_t _width;
    std::size_t _height;
    std::vector<colour> _pixels;
};

} // namespace engine3d::renderer
