#include "engine3d/math/math.hxx"
#include "engine3d/math/vector2.hxx"
#include "engine3d/renderer/rasterizer.hxx"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace {

using e3d::math::vector2;
using e3d::math::vector3;
using e3d::math::cross;
using e3d::math::epsilon;

float edge(const vector2& a, const vector2& b, const vector2& p)
{
    return cross(p - a, b - a);
}

bool is_top_left(vector2 a, vector2 b)
{
    const float dx = b.x() - a.x();
    const float dy = b.y() - a.y();

    return dy > 0 || (dy == 0 && dx < 0);
}

std::tuple<int,int,int,int> bounding_box(vector2 p0, vector2 p1, vector2 p2) noexcept
{
    auto min3 = [](auto a, auto b, auto c) { return std::min(a, std::min(b, c)); };
    auto max3 = [](auto a, auto b, auto c) { return std::max(a, std::max(b, c)); };

    return {
        static_cast<int>(std::floor(min3(p0.x(), p1.x(), p2.x()))),
        static_cast<int>(std::floor(min3(p0.y(), p1.y(), p2.y()))),
        static_cast<int>(std::ceil(max3(p0.x(), p1.x(), p2.x()))),
        static_cast<int>(std::ceil(max3(p0.y(), p1.y(), p2.y())))
    };
}

vector3 barycentric(vector2 p, vector2 a, vector2 b, vector2 c) noexcept
{
    const float area = cross(c - a, b - a);
    if( !std::isfinite(area) || std::fabs(area) < epsilon )
        return {0.0F, 0.0F, 0.0F};
        
        return {
            cross(p - b, c - b) / area,
            cross(p - c, a - c) / area,
            cross(p - a, b - a) / area
    };
}

} // namespace


namespace e3d::renderer {

rasterizer::rasterizer(framebuffer& fb) noexcept
    : _framebuffer{fb}
{}

void rasterizer::draw_line(vector2 p0, vector2 p1, graphics::colour c)
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
        _framebuffer.set_clipped(x0, y0, c);

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

void rasterizer::draw_triangle(vector2 p0, vector2 p1, vector2 p2, graphics::colour c)
{
    const float area = edge(p0, p2, p1);
    if( !std::isfinite(area) || std::abs(area) < math::epsilon )
        return;

    draw_line(p0, p1, c);
    draw_line(p1, p2, c);
    draw_line(p2, p0, c);
}

void rasterizer::draw_filled_triangle(vector2 p0, vector2 p1, vector2 p2, graphics::colour c)
{
    float area = edge(p0, p1, p2);
    if( !std::isfinite(area) || std::fabs(area) < math::epsilon )
        return;

    if( area < 0.0F ) {
        std::swap(p1, p2);
        area = -area;
    }

    auto inside_edge = [](float e, bool top_left) { return e > 0.0F || (e == 0.0F && top_left); };

    const auto tl0 = is_top_left(p1, p2);
    const auto tl1 = is_top_left(p2, p0);
    const auto tl2 = is_top_left(p0, p1);

    const auto [ox, oy, cx, cy] = bounding_box(p0, p1, p2);

    for( int x = ox; x <= cx; ++x )
        for( int y = oy; y <= cy; ++y ) {
            vector2 p{x + 0.5F, y + 0.5F};
            const auto e0 = edge(p1, p2, p);
            const auto e1 = edge(p2, p0, p);
            const auto e2 = edge(p0, p1, p);

            if( !inside_edge(e0, tl0) || !inside_edge(e1, tl1) || !inside_edge(e2, tl2) )
                continue;

            _framebuffer.set_clipped(x, y, c);
        }
}

void rasterizer::draw_filled_triangle(vertex2d v0, vertex2d v1, vertex2d v2)
{
    auto inside_edge = [](float e, bool top_left) { return e > 0.0f || (e == 0.0f && top_left); };

    float area = edge(v0.position, v1.position, v2.position);
    if( !std::isfinite(area) || std::fabs(area) < math::epsilon )
        return;

    if( area < 0.0F ) {
        std::swap(v1, v2);
        area = -area;
    }
    
    assert(area > 0.0F);

    const bool tl0 = is_top_left(v1.position, v2.position);
    const bool tl1 = is_top_left(v2.position, v0.position);
    const bool tl2 = is_top_left(v0.position, v1.position);

    const auto [ox, oy, cx, cy] = bounding_box(v0.position, v1.position, v2.position);

    for( int y = oy; y <= cy; ++y )
        for( int x = ox; x <= cx; ++x ) {
            vector2 p{x + 0.5F, y + 0.5F};

            const float e0 = edge(v1.position, v2.position, p);
            const float e1 = edge(v2.position, v0.position, p);
            const float e2 = edge(v0.position, v1.position, p);

            if( !inside_edge(e0, tl0) || !inside_edge(e1, tl1) || !inside_edge(e2, tl2) )
                continue;

            const float w0 = e0 / area;
            const float w1 = e1 / area;
            const float w2 = e2 / area;
            auto col = v0.colour * w0 + v1.colour * w1 + v2.colour * w2;

            _framebuffer.set_clipped(x, y, col);
        }

}

} // namespace e3d::renderer
