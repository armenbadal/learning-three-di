#pragma once

#include "colour.hxx"

#include <vector>

namespace renderer {

class framebuffer {
public:
    framebuffer(unsigned int width, unsigned int height);

    unsigned int width() const { return _width; }
    unsigned int height() const { return _height; }

    colour get(unsigned int x, unsigned int y) const;
    void set(unsigned int x, unsigned int y, colour p);

private:
    unsigned int _width;
    unsigned int _height;
    std::vector<colour> _colours;
};

} // namespace renderer
