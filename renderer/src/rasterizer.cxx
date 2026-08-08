#include "rasterizer.hxx"
#include "vector2.hxx"

#include <algorithm>
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

void rasterizer::draw_line(math::vector2 p0, math::vector2 p1, colour c)
{
    auto x0r = std::round(p0.x()), y0r = std::round(p0.y());
    auto x1r = std::round(p1.x()), y1r = std::round(p1.y());

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

void rasterizer::draw_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c)
{
    draw_line(p0, p1, c);
    draw_line(p1, p2, c);
    draw_line(p2, p0, c);
}

void rasterizer::draw_filled_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c)
{
    draw_triangle(p0, p1, p2, c);

    const int ox = static_cast<int>(std::floor(std::min(p0.x(), std::min(p1.x(), p2.x()))));
    const int oy = static_cast<int>(std::floor(std::min(p0.y(), std::min(p1.y(), p2.y()))));
    const int cx = static_cast<int>(std::ceil(std::max(p0.x(), std::max(p1.x(), p2.x()))));
    const int cy = static_cast<int>(std::ceil(std::max(p0.y(), std::max(p1.y(), p2.y()))));

    for( int x = ox; x <= cx; ++x )
        for( int y = oy; y <= cy; ++y ) {
            math::vector2 p{x + 0.5F, y + 0.5F};
            const auto e0 = (p - p0).cross_product(p1 - p0);
            const auto e1 = (p - p1).cross_product(p2 - p1);
            const auto e2 = (p - p2).cross_product(p0 - p2);

            if( (e0 >= 0 && e1 >= 0 && e2 >= 0) || (e0 <= 0 && e1 <= 0 && e2 <= 0) ) 
                _framebuffer.set(x, y, c);
        }
}

} // namespace renderer