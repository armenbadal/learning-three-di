#include "engine3d/math/math.hxx"
#include "engine3d/math/vector2.hxx"
#include "engine3d/renderer/software/rasterizer.hxx"

#include <algorithm>
#include <cmath>
#include <tuple>

namespace {

using e3d::math::vector2;
using e3d::math::cross;

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

bool inside_edge(float edge_value, bool top_left)
{
    return edge_value > 0.0F || (edge_value == 0.0F && top_left);
}

std::tuple<int, int, int, int> bounding_box(vector2 p0, vector2 p1, vector2 p2) noexcept
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

std::tuple<int, int, int, int> clipped_bounding_box(
    vector2 p0,
    vector2 p1,
    vector2 p2,
    int width,
    int height) noexcept
{
    auto [min_x, min_y, max_x, max_y] = bounding_box(p0, p1, p2);
    return {
        std::max(min_x, 0),
        std::max(min_y, 0),
        std::min(max_x, width - 1),
        std::min(max_y, height - 1)
    };
}

} // namespace


namespace e3d::renderer {

rasterizer::rasterizer(framebuffer& fb)
    : _framebuffer{fb}
{}

void rasterizer::draw_line(screen_vertex v0, screen_vertex v1, graphics::colour c)
{
    const auto& p0 = v0.position;
    const auto& p1 = v1.position;
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

void rasterizer::draw_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, graphics::colour c)
{
    const float area = edge(v0.position, v2.position, v1.position);
    if( !std::isfinite(area) || std::abs(area) < math::epsilon )
        return;

    draw_line(v0, v1, c);
    draw_line(v1, v2, c);
    draw_line(v2, v0, c);
}

void rasterizer::draw_filled_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, graphics::colour c)
{
    rasterize_filled_triangle(v0, v1, v2, &c);
}

void rasterizer::draw_filled_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2)
{
    rasterize_filled_triangle(v0, v1, v2, nullptr);
}

void rasterizer::rasterize_filled_triangle(
    screen_vertex v0,
    screen_vertex v1,
    screen_vertex v2,
    const graphics::colour* fill_colour)
{
    float area = edge(v0.position, v1.position, v2.position);
    if( !std::isfinite(area) || std::fabs(area) < math::epsilon )
        return;

    if( area < 0.0F ) {
        std::swap(v1, v2);
        area = -area;
    }

    const auto tl0 = is_top_left(v1.position, v2.position);
    const auto tl1 = is_top_left(v2.position, v0.position);
    const auto tl2 = is_top_left(v0.position, v1.position);

    if( _framebuffer.width() == 0 || _framebuffer.height() == 0 )
        return;

    const auto [ox, oy, cx, cy] = clipped_bounding_box(
        v0.position,
        v1.position,
        v2.position,
        static_cast<int>(_framebuffer.width()),
        static_cast<int>(_framebuffer.height()));

    for( int x = ox; x <= cx; ++x )
        for( int y = oy; y <= cy; ++y ) {
            vector2 p{static_cast<float>(x) + 0.5F, static_cast<float>(y) + 0.5F};
            const auto e0 = edge(v1.position, v2.position, p);
            const auto e1 = edge(v2.position, v0.position, p);
            const auto e2 = edge(v0.position, v1.position, p);

            if( !inside_edge(e0, tl0) || !inside_edge(e1, tl1) || !inside_edge(e2, tl2) )
                continue;

            const auto ux = static_cast<std::size_t>(x);
            const auto uy = static_cast<std::size_t>(y);
            const float lambda0 = e0 / area;
            const float lambda1 = e1 / area;
            const float lambda2 = e2 / area;
            const float depth = lambda0 * v0.depth
                              + lambda1 * v1.depth
                              + lambda2 * v2.depth;

            auto& stored_depth = _framebuffer.depth_at(ux, uy);
            if( !(depth < stored_depth) )
                continue;

            if( fill_colour != nullptr ) {
                stored_depth = depth;
                _framebuffer(ux, uy) = *fill_colour;
                continue;
            }

            const float weighted_inv_w = lambda0 * v0.inv_w
                                       + lambda1 * v1.inv_w
                                       + lambda2 * v2.inv_w;
            if( !std::isfinite(weighted_inv_w)
                || std::fabs(weighted_inv_w) < math::epsilon )
                continue;

            const auto colour_over_w = v0.colour_over_w * lambda0
                                     + v1.colour_over_w * lambda1
                                     + v2.colour_over_w * lambda2;

            stored_depth = depth;
            _framebuffer(ux, uy) = graphics::to_colour(colour_over_w / weighted_inv_w);
        }
}

void rasterizer::draw_filled_triangle(vertex2d v0, vertex2d v1, vertex2d v2)
{
    float area = edge(v0.position, v1.position, v2.position);
    if( !std::isfinite(area) || std::fabs(area) < math::epsilon )
        return;

    if( area < 0.0F ) {
        std::swap(v1, v2);
        area = -area;
    }

    const auto colour0 = graphics::to_colourf(v0.colour);
    const auto colour1 = graphics::to_colourf(v1.colour);
    const auto colour2 = graphics::to_colourf(v2.colour);

    const bool tl0 = is_top_left(v1.position, v2.position);
    const bool tl1 = is_top_left(v2.position, v0.position);
    const bool tl2 = is_top_left(v0.position, v1.position);

    if( _framebuffer.width() == 0 || _framebuffer.height() == 0 )
        return;

    const auto [ox, oy, cx, cy] = clipped_bounding_box(
        v0.position,
        v1.position,
        v2.position,
        static_cast<int>(_framebuffer.width()),
        static_cast<int>(_framebuffer.height()));

    for( int y = oy; y <= cy; ++y )
        for( int x = ox; x <= cx; ++x ) {
            vector2 p{static_cast<float>(x) + 0.5F, static_cast<float>(y) + 0.5F};

            const float e0 = edge(v1.position, v2.position, p);
            const float e1 = edge(v2.position, v0.position, p);
            const float e2 = edge(v0.position, v1.position, p);

            if( !inside_edge(e0, tl0) || !inside_edge(e1, tl1) || !inside_edge(e2, tl2) )
                continue;

            const float lambda0 = e0 / area;
            const float lambda1 = e1 / area;
            const float lambda2 = e2 / area;
            const auto colour = graphics::to_colour(
                colour0 * lambda0 + colour1 * lambda1 + colour2 * lambda2);

            _framebuffer(static_cast<std::size_t>(x), static_cast<std::size_t>(y)) = colour;
        }
}

} // namespace e3d::renderer
