#include "rasterizer.hxx"
#include "vector2.hxx"

#include <cmath>

namespace renderer {

rasterizer::rasterizer(framebuffer& fb)
    : _framebuffer{fb}
{}

void rasterizer::draw_line_dda(float x0, float y0, float x1, float y1, colour c)
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

void rasterizer::draw_line(float x0, float y0, float x1, float y1, colour c)
{
    auto x0r = std::round(x0), y0r = std::round(y0);
    auto x1r = std::round(x1), y1r = std::round(y1);

    const int dx = std::abs(x1r - x0r);
    const int slope_x = x0r < x1r ? 1 : -1;

    const int dy = -std::abs(y1r - y0r);
    const int slope_y = y0r < y1r ? 1 : -1;

    int error = dx + dy;

    while( true ) {
        _framebuffer.set(x0r, y0r, c);

        if( x0r == x1r && y0r == y1r )
            break;

        const int e2 = 2 * error;

        if( e2 >= dy ) {
            error += dy;
            x0r += slope_x;
        }

        if( e2 <= dx ) {
            error += dx;
            y0r += slope_y;
        }
    }
}

} // namespace renderer