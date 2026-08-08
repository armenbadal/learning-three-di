#include "rasterizer.hxx"
#include "vector2.hxx"

#include <cmath>

namespace renderer {

rasterizer::rasterizer(framebuffer& fb)
    : _framebuffer{fb}
{}

void rasterizer::draw_line(float x0, float y0, float x1, float y1, colour c)
{
    const auto dx = std::fabs(x0 - x1);
    const auto dy = std::fabs(y0 - y1);
    const int samples = std::ceil(std::max(dx, dy));
    const float xinc = dx / samples;
    const float yinc = dy / samples;

    float x = x0, y = y0;
    for( int i = 0; i <= samples; ++i ) {
        _framebuffer.set(std::round(x), std::round(y), c);
        x += xinc;
        y += yinc;
    }
}

} // namespace renderer
