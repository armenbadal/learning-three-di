#include <catch2/catch_approx.hpp>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>

#include "rasterizer.hxx"

#include <algorithm>
#include <queue>
#include <utility>
#include <vector>

using namespace renderer;

namespace {
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
                if( is_black(fb.get(x, y)) )
                    ++n;
        return n;
    }

    bool within_bounds(const framebuffer& fb, unsigned int minx, unsigned int maxx,
                       unsigned int miny, unsigned int maxy)
    {
        for( unsigned int y = 0; y < fb.height(); ++y )
            for( unsigned int x = 0; x < fb.width(); ++x )
                if( is_black(fb.get(x, y)) && (x < minx || x > maxx || y < miny || y > maxy) )
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
            if( !is_black(fb.get(cx, cy)) )
                continue;
            ++n;
            for( int i = 0; i < 8; ++i ) {
                const int nx = static_cast<int>(cx) + ndx[i];
                const int ny = static_cast<int>(cy) + ndy[i];
                if( nx < 0 || ny < 0 || nx >= static_cast<int>(fb.width())
                    || ny >= static_cast<int>(fb.height()) )
                    continue;
                if( visited[ny][nx] )
                    continue;
                visited[ny][nx] = true;
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
        r.draw_line(math::vector2{static_cast<float>(c.x0), static_cast<float>(c.y0)},
                    math::vector2{static_cast<float>(c.x1), static_cast<float>(c.y1)},
                    black);

        INFO(c.name);
        CHECK(is_black(fb.get(c.x0, c.y0)));
        CHECK(is_black(fb.get(c.x1, c.y1)));

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
    r.draw_triangle(math::vector2{2.0F, 2.0F},
                    math::vector2{18.0F, 2.0F},
                    math::vector2{6.0F, 12.0F},
                    black);

    CHECK(is_black(fb.get(2, 2)));
    CHECK(is_black(fb.get(18, 2)));
    CHECK(is_black(fb.get(6, 12)));

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
    r.draw_filled_triangle(math::vector2{0.0F, 0.0F},
                           math::vector2{4.0F, 0.0F},
                           math::vector2{0.0F, 4.0F},
                           black);

    const unsigned int black_per_row[] = {4, 3, 2, 1, 0};
    for( unsigned int y = 0; y < 5; ++y ) {
        for( unsigned int x = 0; x < 6; ++x ) {
            const bool expect = x < black_per_row[y];
            INFO("x=" << x << " y=" << y);
            CHECK(is_black(fb.get(x, y)) == expect);
        }
    }

    CHECK(is_black(fb.get(1, 1)));
    CHECK_FALSE(is_black(fb.get(5, 5)));
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

    CHECK(fb.get(0, 0) == colour{180, 30, 30});
    CHECK(fb.get(3, 0) == colour{0, 210, 30});
    CHECK(fb.get(0, 3) == colour{0, 30, 210});
    CHECK(fb.get(1, 1) == colour{60, 90, 90});

    CHECK(fb.get(5, 5) == white);
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

    const unsigned int filled_per_row[] = {4, 3, 2, 1, 0};
    for( unsigned int y = 0; y < 5; ++y ) {
        for( unsigned int x = 0; x < 6; ++x ) {
            const bool expect = x < filled_per_row[y];
            const bool filled = fb.get(x, y) != white;
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

    r.draw_triangle(math::vector2{0.0F, 0.0F},
                    math::vector2{0.0F, 0.0F},
                    math::vector2{0.0F, 0.0F},
                    black);

    CHECK(count_black(fb) == 0);
}

namespace {
    void check_barycentric(math::vector2 p, math::vector2 a, math::vector2 b, math::vector2 c,
                           float wa, float wb, float wc)
    {
        const auto w = rasterizer::barycentric(p, a, b, c);
        CHECK(w.x() == Catch::Approx(wa).margin(1e-5f));
        CHECK(w.y() == Catch::Approx(wb).margin(1e-5f));
        CHECK(w.z() == Catch::Approx(wc).margin(1e-5f));
    }
}

TEST_CASE("barycentric at vertices")
{
    const math::vector2 a{0.0F, 0.0F};
    const math::vector2 b{10.0F, 0.0F};
    const math::vector2 c{0.0F, 10.0F};

    check_barycentric(a, a, b, c, 1.0F, 0.0F, 0.0F);
    check_barycentric(b, a, b, c, 0.0F, 1.0F, 0.0F);
    check_barycentric(c, a, b, c, 0.0F, 0.0F, 1.0F);
}

TEST_CASE("barycentric edge and interior points")
{
    const math::vector2 a{0.0F, 0.0F};
    const math::vector2 b{10.0F, 0.0F};
    const math::vector2 c{0.0F, 10.0F};

    check_barycentric({5.0F, 0.0F}, a, b, c, 0.5F, 0.5F, 0.0F);
    check_barycentric({0.0F, 5.0F}, a, b, c, 0.5F, 0.0F, 0.5F);
    check_barycentric({5.0F, 2.0F}, a, b, c, 0.3F, 0.5F, 0.2F);
    check_barycentric({10.0F / 3.0F, 10.0F / 3.0F}, a, b, c, 1.0F / 3.0F, 1.0F / 3.0F, 1.0F / 3.0F);
}

TEST_CASE("barycentric weights sum to one")
{
    const math::vector2 a{0.0F, 0.0F};
    const math::vector2 b{10.0F, 0.0F};
    const math::vector2 c{0.0F, 10.0F};

    for( int i = 0; i < 11; ++i ) {
        const math::vector2 p{static_cast<float>(i), static_cast<float>(10 - i)};
        const auto w = rasterizer::barycentric(p, a, b, c);
        CHECK(w.x() + w.y() + w.z() == Catch::Approx(1.0F).margin(1e-5f));
    }
}

TEST_CASE("barycentric outside point has a negative weight")
{
    const math::vector2 a{0.0F, 0.0F};
    const math::vector2 b{10.0F, 0.0F};
    const math::vector2 c{0.0F, 10.0F};

    const auto w = rasterizer::barycentric({20.0F, 5.0F}, a, b, c);
    const bool has_negative = w.x() < 0.0F || w.y() < 0.0F || w.z() < 0.0F;
    CHECK(has_negative);
    CHECK(w.x() + w.y() + w.z() == Catch::Approx(1.0F).margin(1e-5f));
}

TEST_CASE("barycentric reconstructs point")
{
    const math::vector2 a{0.0F, 0.0F};
    const math::vector2 b{10.0F, 0.0F};
    const math::vector2 c{0.0F, 10.0F};
    const math::vector2 p{3.0F, 7.0F};

    const auto w = rasterizer::barycentric(p, a, b, c);
    const math::vector2 r = a * w.x() + b * w.y() + c * w.z();
    CHECK(r.x() == Catch::Approx(p.x()).margin(1e-5f));
    CHECK(r.y() == Catch::Approx(p.y()).margin(1e-5f));
}

TEST_CASE("barycentric degenerate triangle returns zero")
{
    const math::vector2 a{0.0F, 0.0F};
    const math::vector2 b{1.0F, 1.0F};
    const math::vector2 c{2.0F, 2.0F};

    const auto w = rasterizer::barycentric({1.0F, 1.0F}, a, b, c);
    CHECK(w.x() == 0.0F);
    CHECK(w.y() == 0.0F);
    CHECK(w.z() == 0.0F);
}

TEST_CASE("bounding_box integer vertices")
{
    const auto [origin, corner] = rasterizer::bounding_box(
        math::vector2{0.0F, 0.0F}, math::vector2{10.0F, 0.0F}, math::vector2{5.0F, 10.0F});

    CHECK(origin.x() == 0.0F);
    CHECK(origin.y() == 0.0F);
    CHECK(corner.x() == 10.0F);
    CHECK(corner.y() == 10.0F);
}

TEST_CASE("bounding_box fractional vertices")
{
    const auto [origin, corner] = rasterizer::bounding_box(
        math::vector2{1.2F, 2.3F}, math::vector2{4.7F, 1.9F}, math::vector2{3.1F, 5.8F});

    CHECK(origin.x() == 1.0F);
    CHECK(origin.y() == 1.0F);
    CHECK(corner.x() == 5.0F);
    CHECK(corner.y() == 6.0F);
}

TEST_CASE("bounding_box negative vertices")
{
    const auto [origin, corner] = rasterizer::bounding_box(
        math::vector2{-2.5F, -3.5F}, math::vector2{1.5F, 2.5F}, math::vector2{0.0F, 0.0F});

    CHECK(origin.x() == -3.0F);
    CHECK(origin.y() == -4.0F);
    CHECK(corner.x() == 2.0F);
    CHECK(corner.y() == 3.0F);
}

TEST_CASE("bounding_box contains vertices")
{
    const math::vector2 a{2.0F, 1.0F};
    const math::vector2 b{7.0F, 3.0F};
    const math::vector2 c{4.0F, 8.0F};

    const auto [origin, corner] = rasterizer::bounding_box(a, b, c);

    CHECK(origin.x() <= a.x());
    CHECK(origin.x() <= b.x());
    CHECK(origin.x() <= c.x());
    CHECK(corner.x() >= a.x());
    CHECK(corner.x() >= b.x());
    CHECK(corner.x() >= c.x());
    CHECK(origin.y() <= a.y());
    CHECK(origin.y() <= b.y());
    CHECK(origin.y() <= c.y());
    CHECK(corner.y() >= a.y());
    CHECK(corner.y() >= b.y());
    CHECK(corner.y() >= c.y());
}

TEST_CASE("bounding_box single point")
{
    const auto [origin, corner] = rasterizer::bounding_box(
        math::vector2{3.0F, 3.0F}, math::vector2{3.0F, 3.0F}, math::vector2{3.0F, 3.0F});

    CHECK(origin.x() == 3.0F);
    CHECK(origin.y() == 3.0F);
    CHECK(corner.x() == 3.0F);
    CHECK(corner.y() == 3.0F);
}