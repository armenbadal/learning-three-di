#include "rasterizer.hxx"
#include "vector2.hxx"
#include "math.hxx"

#include <algorithm>
#include <cmath>

namespace renderer {

rasterizer::rasterizer(framebuffer& fb)
    : _framebuffer{fb}
{}

void rasterizer::draw_line(math::vector2 p0, math::vector2 p1, colour c)
{
    int x0 = static_cast<int>(std::round(p0.x()));
    int y0 = static_cast<int>(std::round(p0.y()));
    int x1 = static_cast<int>(std::round(p1.x()));
    int y1 = static_cast<int>(std::round(p1.y()));

    const int dx = std::abs(x1 - x0);
    const int slope_x = x0 < x1 ? 1 : -1;

    const int dy = -std::abs(y1 - y0);
    const int slope_y = y0 < y1 ? 1 : -1;

    int error = dx + dy;

    while( true ) {
        _framebuffer.set(x0, y0, c);

        if( x0 == x1 && y0 == y1 )
            break;

        const int e2 = 2 * error;

        if( e2 >= dy ) {
            error += dy;
            x0 += slope_x;
        }

        if( e2 <= dx ) {
            error += dx;
            y0 += slope_y;
        }
    }
}

void rasterizer::draw_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c)
{
    if( std::abs((p1 - p0).cross_product(p2 - p0)) < math::epsilon )
        return;

    draw_line(p0, p1, c);
    draw_line(p1, p2, c);
    draw_line(p2, p0, c);
}

void rasterizer::draw_filled_triangle(math::vector2 p0, math::vector2 p1, math::vector2 p2, colour c)
{
    const auto [origin, corner] = bounding_box(p0, p1, p2);
    const int ox = static_cast<int>(origin.x());
    const int oy = static_cast<int>(origin.y());
    const int cx = static_cast<int>(corner.x());
    const int cy = static_cast<int>(corner.y());

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

void rasterizer::draw_filled_triangle(vertex2d v0, vertex2d v1, vertex2d v2)
{
    const auto [origin, corner] = bounding_box(v0._position, v1._position, v2._position);
    const int ox = static_cast<int>(origin.x());
    const int oy = static_cast<int>(origin.y());
    const int cx = static_cast<int>(corner.x());
    const int cy = static_cast<int>(corner.y());

    for( int x = ox; x <= cx; ++x )
        for( int y = oy; y <= cy; ++y ) {
            math::vector2 p{x + 0.5F, y + 0.5F};
            auto bc = barycentric(p, v0._position, v1._position, v2._position);
            const auto pos = bc.x() >= 0 && bc.y() >= 0 && bc.z() >= 0;
            const auto neg = bc.x() <= 0 && bc.y() <= 0 && bc.z() <= 0;
            if( pos || neg ) {
                auto col = v0._colour * bc.x() + v1._colour * bc.y() + v2._colour * bc.z();
                _framebuffer.set(x, y, col);
            }
        }

}

std::tuple<math::vector2, math::vector2> rasterizer::bounding_box(math::vector2 p0, math::vector2 p1, math::vector2 p2)
{
    auto min3 = [](auto a, auto b, auto c) { return std::min(a, std::min(b, c)); };
    auto max3 = [](auto a, auto b, auto c) { return std::max(a, std::max(b, c)); };

    math::vector2 origin{
        std::floor(min3(p0.x(), p1.x(), p2.x())),
        std::floor(min3(p0.y(), p1.y(), p2.y()))
    };
    math::vector2 corner{
        std::ceil(max3(p0.x(), p1.x(), p2.x())),
        std::ceil(max3(p0.y(), p1.y(), p2.y()))
    };
    return {origin, corner};
}

math::vector3 rasterizer::barycentric(math::vector2 p, math::vector2 a, math::vector2 b, math::vector2 c)
{
    const float area = (c - a).cross_product(b - a);
    if( std::fabs(area) < math::epsilon )
        return {0.0F, 0.0F, 0.0F};

    return {
        (p - b).cross_product(c - b) / area,
        (p - c).cross_product(a - c) / area,
        (p - a).cross_product(b - a) / area
    };
}

} // namespace renderer