#pragma once

#include "colour.hxx"

#include <cstddef>
#include <vector>

namespace renderer {

class framebuffer {
public:
    framebuffer(std::size_t width, std::size_t height);

    std::size_t width() const { return _width; }
    std::size_t height() const { return _height; }

    // Checked access; throws std::out_of_range for coordinates outside the buffer.
    colour& at(std::size_t x, std::size_t y);
    const colour& at(std::size_t x, std::size_t y) const;

    // Unchecked access; caller must keep coordinates in bounds.
    colour& operator()(std::size_t x, std::size_t y);
    const colour& operator()(std::size_t x, std::size_t y) const;

    // Clipped write; out-of-bounds coordinates are silently ignored.
    void set_clipped(int x, int y, colour p);

    void clear(colour c = black);

private:
    std::size_t _width;
    std::size_t _height;
    std::vector<colour> _pixels;
};

} // namespace renderer
