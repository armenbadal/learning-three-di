#pragma once

#include "colour.hxx"
#include "framebuffer.hxx"

namespace renderer {

class rasterizer {
public:
    rasterizer(framebuffer& fb);

    void draw_line(float x0, float y0, float x1, float y1, colour c);

private:
    framebuffer& _framebuffer;
};

} // namespace renderer
