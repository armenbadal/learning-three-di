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
    framebuffer fb{20, 20};
    rasterizer r{fb};
    r.draw_filled_triangle(math::vector2{0.0F, 0.0F},
                           math::vector2{10.0F, 0.0F},
                           math::vector2{5.0F, 10.0F},
                           black);

    CHECK(is_black(fb.get(0, 0)));
    CHECK(is_black(fb.get(10, 0)));
    CHECK(is_black(fb.get(5, 10)));

    for( unsigned int x = 2; x <= 8; ++x )
        CHECK(is_black(fb.get(x, 4)));

    CHECK(is_black(fb.get(5, 3)));
    CHECK(is_black(fb.get(5, 9)));

    CHECK_FALSE(is_black(fb.get(0, 4)));
    CHECK_FALSE(is_black(fb.get(9, 4)));
    CHECK_FALSE(is_black(fb.get(5, 11)));
    CHECK_FALSE(is_black(fb.get(10, 10)));
}