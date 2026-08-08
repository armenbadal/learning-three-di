#include <catch2/catch_test_macros.hpp>

#include "rasterizer.hxx"

using namespace renderer;

TEST_CASE("draw_line horizontal")
{
    framebuffer fb{10, 10};
    rasterizer r{fb};

    r.draw_line(0.0F, 5.0F, 9.0F, 5.0F, black);

    for( unsigned int x = 0; x < 10; ++x )
        CHECK(fb.get(x, 5).r() == 0);

    CHECK(fb.get(0, 4).r() == 255);
    CHECK(fb.get(9, 6).r() == 255);
}

TEST_CASE("draw_line vertical")
{
    framebuffer fb{10, 10};
    rasterizer r{fb};

    r.draw_line(3.0F, 0.0F, 3.0F, 9.0F, black);

    for( unsigned int y = 0; y < 10; ++y )
        CHECK(fb.get(3, y).r() == 0);

    CHECK(fb.get(2, 0).r() == 255);
}

TEST_CASE("draw_line diagonal includes endpoints")
{
    framebuffer fb{10, 10};
    rasterizer r{fb};

    r.draw_line(0.0F, 0.0F, 9.0F, 9.0F, black);

    CHECK(fb.get(0, 0).r() == 0);
    CHECK(fb.get(9, 9).r() == 0);
    CHECK(fb.get(9, 0).r() == 255);
    CHECK(fb.get(0, 9).r() == 255);
}

TEST_CASE("draw_line single point")
{
    framebuffer fb{5, 5};
    rasterizer r{fb};

    r.draw_line(2.0F, 2.0F, 2.0F, 2.0F, black);

    CHECK(fb.get(2, 2).r() == 0);
    CHECK(fb.get(2, 3).r() == 255);
}