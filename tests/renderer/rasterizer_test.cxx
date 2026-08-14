#include <catch2/catch_approx.hpp>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>

import e3d;

#include <algorithm>
#include <queue>
#include <utility>
#include <vector>

namespace math = e3d::math;
using namespace e3d::renderer;
using namespace e3d::geometry;
using namespace e3d::graphics;

namespace {
    screen_vertex screen(float x, float y, float depth = 0.5F)
    {
        return {{x, y}, depth};
    }

    screen_vertex screen(math::vector2 position, float depth = 0.5F)
    {
        return {position, depth};
    }

    screen_vertex coloured_screen(
        float x,
        float y,
        float inv_w,
        colour c,
        float depth = 0.5F)
    {
        return {
            .position = {x, y},
            .depth = depth,
            .inv_w = inv_w,
            .colour_over_w = to_colourf(c) * inv_w
        };
    }

    struct line_case {
        const char* name;
        unsigned int x0;
        unsigned int y0;
        unsigned int x1;
        unsigned int y1;
        std::size_t expected_pixels;
    };

    bool is_black(const colour& c)
    {
        return c.r() == 0 && c.g() == 0 && c.b() == 0;
    }

    std::size_t count_black(const framebuffer& fb)
    {
        std::size_t n = 0;
        for( unsigned int y = 0; y < fb.height(); ++y )
            for( unsigned int x = 0; x < fb.width(); ++x )
                if( is_black(fb.at(x, y)) )
                    ++n;
        return n;
    }

    bool within_bounds(const framebuffer& fb, unsigned int minx, unsigned int maxx,
                       unsigned int miny, unsigned int maxy)
    {
        for( unsigned int y = 0; y < fb.height(); ++y )
            for( unsigned int x = 0; x < fb.width(); ++x )
                if( is_black(fb.at(x, y)) && (x < minx || x > maxx || y < miny || y > maxy) )
                    return false;
        return true;
    }

    std::size_t reachable_black(const framebuffer& fb, unsigned int sx, unsigned int sy)
    {
        std::vector<std::vector<bool>> visited(fb.height(), std::vector<bool>(fb.width(), false));
        std::queue<std::pair<unsigned int, unsigned int>> q;
        q.push({sx, sy});
        visited[sy][sx] = true;

        const int ndx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        const int ndy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

        std::size_t n = 0;
        while( !q.empty() ) {
            const auto [cx, cy] = q.front();
            q.pop();
            if( !is_black(fb.at(cx, cy)) )
                continue;
            ++n;
            for( int i = 0; i < 8; ++i ) {
                const int nx = static_cast<int>(cx) + ndx[i];
                const int ny = static_cast<int>(cy) + ndy[i];
                if( nx < 0 || ny < 0 || nx >= static_cast<int>(fb.width())
                    || ny >= static_cast<int>(fb.height()) )
                    continue;
                const auto ux = static_cast<std::size_t>(nx);
                const auto uy = static_cast<std::size_t>(ny);
                if( visited[uy][ux] )
                    continue;
                visited[uy][ux] = true;
                q.push({static_cast<unsigned int>(nx), static_cast<unsigned int>(ny)});
            }
        }
        return n;
    }

    void check_line(const line_case& c)
    {
        framebuffer fb{20, 20};
        fb.clear(white);
        rasterizer r{fb};
        r.draw_line(screen(static_cast<float>(c.x0), static_cast<float>(c.y0)),
                    screen(static_cast<float>(c.x1), static_cast<float>(c.y1)),
                    black);

        INFO(c.name);
        CHECK(is_black(fb.at(c.x0, c.y0)));
        CHECK(is_black(fb.at(c.x1, c.y1)));

        const auto minx = std::min(c.x0, c.x1);
        const auto maxx = std::max(c.x0, c.x1);
        const auto miny = std::min(c.y0, c.y1);
        const auto maxy = std::max(c.y0, c.y1);

        CHECK(within_bounds(fb, minx, maxx, miny, maxy));
        CHECK(count_black(fb) == c.expected_pixels);
        CHECK(reachable_black(fb, c.x0, c.y0) == c.expected_pixels);
    }
}

TEST_CASE("draw_line horizontal")
{
    check_line({"horizontal", 0, 5, 9, 5, 10});
}

TEST_CASE("draw_line vertical")
{
    check_line({"vertical", 3, 0, 3, 9, 10});
}

TEST_CASE("draw_line 45 degrees")
{
    check_line({"45 degrees", 0, 0, 9, 9, 10});
}

TEST_CASE("draw_line shallow positive slope")
{
    check_line({"shallow positive", 0, 0, 8, 3, 9});
}

TEST_CASE("draw_line steep positive slope")
{
    check_line({"steep positive", 0, 0, 3, 8, 9});
}

TEST_CASE("draw_line shallow negative slope")
{
    check_line({"shallow negative", 0, 8, 8, 5, 9});
}

TEST_CASE("draw_line steep negative slope")
{
    check_line({"steep negative", 0, 8, 3, 0, 9});
}

TEST_CASE("draw_line zero length")
{
    check_line({"zero length", 4, 4, 4, 4, 1});
}

TEST_CASE("draw_triangle outline")
{
    framebuffer fb{20, 20};
    fb.clear(white);
    rasterizer r{fb};
    r.draw_triangle(screen(2.0F, 2.0F),
                    screen(18.0F, 2.0F),
                    screen(6.0F, 12.0F),
                    black);

    CHECK(is_black(fb.at(2, 2)));
    CHECK(is_black(fb.at(18, 2)));
    CHECK(is_black(fb.at(6, 12)));

    const std::size_t expected = 17 + 13 + 11 - 3;
    CHECK(count_black(fb) == expected);
    CHECK(within_bounds(fb, 2, 18, 2, 12));
    CHECK(reachable_black(fb, 2, 2) == expected);
}

TEST_CASE("draw_filled_triangle fills interior")
{
    framebuffer fb{6, 5};
    fb.clear(white);
    rasterizer r{fb};
    r.draw_filled_triangle(screen(0.0F, 0.0F),
                           screen(4.0F, 0.0F),
                           screen(0.0F, 4.0F),
                           black);

    const unsigned int black_per_row[] = {3, 2, 1, 0, 0};
    for( unsigned int y = 0; y < 5; ++y ) {
        for( unsigned int x = 0; x < 6; ++x ) {
            const bool expect = x < black_per_row[y];
            INFO("x=" << x << " y=" << y);
            CHECK(is_black(fb.at(x, y)) == expect);
        }
    }

    CHECK(is_black(fb.at(1, 1)));
    CHECK_FALSE(is_black(fb.at(5, 4)));
}

TEST_CASE("draw_filled_triangle depth is independent of draw order")
{
    const screen_vertex v0_near = screen(0.0F, 0.0F, 0.2F);
    const screen_vertex v1_near = screen(4.0F, 0.0F, 0.2F);
    const screen_vertex v2_near = screen(0.0F, 4.0F, 0.2F);
    const screen_vertex v0_far = screen(0.0F, 0.0F, 0.8F);
    const screen_vertex v1_far = screen(4.0F, 0.0F, 0.8F);
    const screen_vertex v2_far = screen(0.0F, 4.0F, 0.8F);
    const colour near_colour{255, 0, 0};
    const colour far_colour{0, 0, 255};

    framebuffer near_first{5, 5};
    framebuffer far_first{5, 5};
    near_first.clear(white);
    far_first.clear(white);
    rasterizer a{near_first};
    rasterizer b{far_first};

    a.draw_filled_triangle(v0_near, v1_near, v2_near, near_colour);
    a.draw_filled_triangle(v0_far, v1_far, v2_far, far_colour);
    b.draw_filled_triangle(v0_far, v1_far, v2_far, far_colour);
    b.draw_filled_triangle(v0_near, v1_near, v2_near, near_colour);

    CHECK(std::ranges::equal(near_first.pixels(), far_first.pixels()));
    CHECK(std::ranges::equal(near_first.depths(), far_first.depths()));
    CHECK(near_first.at(1, 1) == near_colour);
    CHECK(near_first.depth_at(1, 1) == Catch::Approx(0.2F));
}

TEST_CASE("draw_filled_triangle rejects a farther fragment")
{
    framebuffer fb{5, 5};
    rasterizer r{fb};
    const colour stored_colour{255, 0, 0};
    const colour far_colour{0, 0, 255};
    fb.at(1, 1) = stored_colour;
    fb.depth_at(1, 1) = 0.3F;

    r.draw_filled_triangle(
        screen(0.0F, 0.0F, 0.7F),
        screen(4.0F, 0.0F, 0.7F),
        screen(0.0F, 4.0F, 0.7F),
        far_colour);

    CHECK(fb.at(1, 1) == stored_colour);
    CHECK(fb.depth_at(1, 1) == Catch::Approx(0.3F));
}

TEST_CASE("draw_filled_triangle accepts a nearer fragment")
{
    framebuffer fb{5, 5};
    rasterizer r{fb};
    const colour stored_colour{255, 0, 0};
    const colour near_colour{0, 0, 255};
    fb.at(1, 1) = stored_colour;
    fb.depth_at(1, 1) = 0.7F;

    r.draw_filled_triangle(
        screen(0.0F, 0.0F, 0.3F),
        screen(4.0F, 0.0F, 0.3F),
        screen(0.0F, 4.0F, 0.3F),
        near_colour);

    CHECK(fb.at(1, 1) == near_colour);
    CHECK(fb.depth_at(1, 1) == Catch::Approx(0.3F));
}

TEST_CASE("draw_filled_triangle interpolates depth with barycentric weights")
{
    framebuffer fb{5, 5};
    rasterizer r{fb};

    r.draw_filled_triangle(
        screen(0.0F, 0.0F, 0.1F),
        screen(4.0F, 0.0F, 0.5F),
        screen(0.0F, 4.0F, 0.9F),
        white);

    CHECK(fb.depth_at(0, 0) == Catch::Approx(0.25F));
}

TEST_CASE("perspective colour interpolation reduces to affine interpolation for equal W")
{
    framebuffer fb{5, 5};
    rasterizer r{fb};

    r.draw_filled_triangle(
        coloured_screen(0.0F, 0.0F, 0.5F, {240, 0, 0}),
        coloured_screen(4.0F, 0.0F, 0.5F, {0, 240, 0}),
        coloured_screen(0.0F, 4.0F, 0.5F, {0, 0, 240}));

    CHECK(fb.at(0, 0) == colour{180, 30, 30});
}

TEST_CASE("perspective colour interpolation accounts for unequal W")
{
    framebuffer fb{5, 5};
    rasterizer r{fb};

    r.draw_filled_triangle(
        coloured_screen(0.0F, 0.0F, 1.0F, {0, 0, 0}),
        coloured_screen(4.0F, 0.0F, 0.5F, {200, 0, 0}),
        coloured_screen(0.0F, 4.0F, 0.25F, {200, 0, 0}));

    CHECK(fb.at(0, 0) == colour{22, 0, 0});
    CHECK(fb.at(0, 0) != colour{50, 0, 0});
}

TEST_CASE("perspective colour interpolation preserves equal attributes for unequal W")
{
    framebuffer fb{5, 5};
    rasterizer r{fb};
    const colour expected{12, 34, 56, 78};

    r.draw_filled_triangle(
        coloured_screen(0.0F, 0.0F, 1.0F, expected),
        coloured_screen(4.0F, 0.0F, 0.5F, expected),
        coloured_screen(0.0F, 4.0F, 0.25F, expected));

    CHECK(fb.at(0, 0) == expected);
    CHECK(fb.at(1, 1) == expected);
}

TEST_CASE("draw_filled_triangle interpolates colours")
{
    framebuffer fb{6, 6};
    fb.clear(white);
    rasterizer r{fb};

    const vertex2d v0{{0.0F, 0.0F}, {240, 0, 0}};
    const vertex2d v1{{4.0F, 0.0F}, {0, 240, 0}};
    const vertex2d v2{{0.0F, 4.0F}, {0, 0, 240}};
    r.draw_filled_triangle(v0, v1, v2);

    CHECK(fb.at(0, 0) == colour{180, 30, 30});
    CHECK(fb.at(2, 0) == colour{60, 150, 30});
    CHECK(fb.at(0, 2) == colour{60, 30, 150});
    CHECK(fb.at(1, 1) == colour{60, 90, 90});

    CHECK(fb.at(5, 5) == white);
}

TEST_CASE("draw_filled_triangle colours every interior pixel")
{
    framebuffer fb{6, 5};
    fb.clear(white);
    rasterizer r{fb};

    const vertex2d v0{{0.0F, 0.0F}, {240, 0, 0}};
    const vertex2d v1{{4.0F, 0.0F}, {0, 240, 0}};
    const vertex2d v2{{0.0F, 4.0F}, {0, 0, 240}};
    r.draw_filled_triangle(v0, v1, v2);

    const unsigned int filled_per_row[] = {3, 2, 1, 0, 0};
    for( unsigned int y = 0; y < 5; ++y ) {
        for( unsigned int x = 0; x < 6; ++x ) {
            const bool expect = x < filled_per_row[y];
            const bool filled = fb.at(x, y) != white;
            INFO("x=" << x << " y=" << y);
            CHECK(filled == expect);
        }
    }
}

TEST_CASE("draw_triangle degenerate is skipped")
{
    framebuffer fb{20, 20};
    fb.clear(white);
    rasterizer r{fb};

    r.draw_triangle(screen(0.0F, 0.0F),
                    screen(0.0F, 0.0F),
                    screen(0.0F, 0.0F),
                    black);

    CHECK(count_black(fb) == 0);
}

namespace {
    bool same_framebuffer(const framebuffer& lhs, const framebuffer& rhs)
    {
        if( lhs.width() != rhs.width() || lhs.height() != rhs.height() )
            return false;
        for( unsigned int y = 0; y < lhs.height(); ++y )
            for( unsigned int x = 0; x < lhs.width(); ++x )
                if( lhs.at(x, y) != rhs.at(x, y) )
                    return false;
        return true;
    }
}

TEST_CASE("draw_filled_triangle degenerate collinear draws nothing")
{
    framebuffer fb{20, 20};
    fb.clear(white);
    rasterizer r{fb};
    r.draw_filled_triangle(screen(1.0F, 1.0F),
                           screen(5.0F, 5.0F),
                           screen(9.0F, 9.0F),
                           black);

    for( unsigned int y = 0; y < fb.height(); ++y )
        for( unsigned int x = 0; x < fb.width(); ++x ) {
            INFO("x=" << x << " y=" << y);
            CHECK(fb.at(x, y) == white);
        }
}

TEST_CASE("draw_filled_triangle degenerate closed draws nothing")
{
    framebuffer fb{20, 20};
    fb.clear(white);
    rasterizer r{fb};
    r.draw_filled_triangle(screen(4.0F, 4.0F),
                           screen(4.0F, 4.0F),
                           screen(4.0F, 4.0F),
                           black);

    for( unsigned int y = 0; y < fb.height(); ++y )
        for( unsigned int x = 0; x < fb.width(); ++x ) {
            INFO("x=" << x << " y=" << y);
            CHECK(fb.at(x, y) == white);
        }
}

TEST_CASE("draw_filled_triangle vertex degenerate draws nothing")
{
    framebuffer fb{20, 20};
    fb.clear(white);
    rasterizer r{fb};

    const vertex2d v0{{1.0F, 1.0F}, {255, 0, 0}};
    const vertex2d v1{{5.0F, 5.0F}, {0, 255, 0}};
    const vertex2d v2{{9.0F, 9.0F}, {0, 0, 255}};
    r.draw_filled_triangle(v0, v1, v2);

    for( unsigned int y = 0; y < fb.height(); ++y )
        for( unsigned int x = 0; x < fb.width(); ++x ) {
            INFO("x=" << x << " y=" << y);
            CHECK(fb.at(x, y) == white);
        }
}

TEST_CASE("draw_filled_triangle winding does not change pixels")
{
    const math::vector2 p0{1.0F, 1.0F};
    const math::vector2 p1{18.0F, 3.0F};
    const math::vector2 p2{5.0F, 15.0F};

    framebuffer cw{20, 20};
    framebuffer ccw{20, 20};
    cw.clear(white);
    ccw.clear(white);
    rasterizer a{cw};
    rasterizer b{ccw};
    a.draw_filled_triangle(screen(p0), screen(p1), screen(p2), black);
    b.draw_filled_triangle(screen(p2), screen(p1), screen(p0), black);

    CHECK(same_framebuffer(cw, ccw));
}

TEST_CASE("draw_filled_triangle vertex winding does not change pixels or colours")
{
    const vertex2d va{{1.0F, 1.0F}, {255, 0, 0}};
    const vertex2d vb{{18.0F, 3.0F}, {0, 255, 0}};
    const vertex2d vc{{5.0F, 15.0F}, {0, 0, 255}};

    framebuffer cw{20, 20};
    framebuffer ccw{20, 20};
    cw.clear(white);
    ccw.clear(white);
    rasterizer a{cw};
    rasterizer b{ccw};
    a.draw_filled_triangle(va, vb, vc);
    b.draw_filled_triangle(vc, vb, va);

    CHECK(same_framebuffer(cw, ccw));
}

TEST_CASE("draw_line clips writes to framebuffer")
{
    framebuffer fb{5, 5};
    fb.clear(white);
    rasterizer r{fb};

    r.draw_line(screen(-1000.0F, 2.0F), screen(1000.0F, 2.0F), black);

    CHECK(count_black(fb) == 5);
}

namespace {
    const screen_vertex square_a{{1.0F, 1.0F}, 0.5F};
    const screen_vertex square_b{{8.0F, 1.0F}, 0.5F};
    const screen_vertex square_c{{8.0F, 8.0F}, 0.5F};
    const screen_vertex square_d{{1.0F, 8.0F}, 0.5F};
    const colour square_red{255, 0, 0};
    const colour square_green{0, 255, 0};
}

TEST_CASE("draw_filled_triangle square shared diagonal is order independent")
{
    framebuffer red_first{10, 10};
    framebuffer green_first{10, 10};
    red_first.clear(white);
    green_first.clear(white);
    rasterizer a{red_first};
    rasterizer b{green_first};

    a.draw_filled_triangle(square_a, square_b, square_c, square_red);
    a.draw_filled_triangle(square_a, square_c, square_d, square_green);

    b.draw_filled_triangle(square_a, square_c, square_d, square_green);
    b.draw_filled_triangle(square_a, square_b, square_c, square_red);

    CHECK(same_framebuffer(red_first, green_first));
}

TEST_CASE("draw_filled_triangle shared diagonal belongs to exactly one triangle")
{
    framebuffer fb{10, 10};
    fb.clear(white);
    rasterizer r{fb};
    r.draw_filled_triangle(square_a, square_b, square_c, square_red);
    r.draw_filled_triangle(square_a, square_c, square_d, square_green);

    for( unsigned int y = 0; y < fb.height(); ++y )
        for( unsigned int x = 0; x < fb.width(); ++x ) {
            const colour at = fb.at(x, y);
            if( at == white )
                continue;
            INFO("x=" << x << " y=" << y);
            CHECK((at == square_red || at == square_green));
        }

    for( unsigned int i = 1; i <= 7; ++i ) {
        INFO("diagonal sample x=" << i << " y=" << i);
        const colour at = fb.at(i, i);
        CHECK(at != white);
        CHECK((at == square_red || at == square_green));
    }
}
