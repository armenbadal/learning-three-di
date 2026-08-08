#pragma once

#include "pixel.hxx"

#include <vector>

namespace renderer {

class framebuffer {
public:
    framebuffer(unsigned int width, unsigned int height);

    unsigned int width() const { return _width; }
    unsigned int height() const { return _height; }

    pixel get(unsigned int x, unsigned int y) const;
    void set(unsigned int x, unsigned int y, pixel p);

private:
    unsigned int _width;
    unsigned int _height;
    std::vector<pixel> _pixels;
};

} // namespace renderer
