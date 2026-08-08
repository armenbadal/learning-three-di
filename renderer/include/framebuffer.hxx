#pragma once

#include "colour.hxx"

#include <vector>

namespace renderer {

class framebuffer {
public:
    framebuffer(unsigned int width, unsigned int height);

    unsigned int width() const { return _width; }
    unsigned int height() const { return _height; }

    colour get(int x, int y) const;
    void set(int x, int y, colour p);

    void clear(colour c = black);

private:
    unsigned int _width;
    unsigned int _height;
    std::vector<colour> _pixels;
};

} // namespace renderer
