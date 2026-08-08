#include <catch2/catch_test_macros.hpp>

#include "framebuffer.hxx"

using namespace renderer;

namespace {
    void check_colour(const colour& p, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
    {
        CHECK(p.r() == r);
        CHECK(p.g() == g);
        CHECK(p.b() == b);
        CHECK(p.a() == a);
    }
}

TEST_CASE("framebuffer dimensions")
{
    const framebuffer fb{640, 480};

    CHECK(fb.width() == 640);
    CHECK(fb.height() == 480);
}

TEST_CASE("framebuffer is cleared after construction")
{
    const framebuffer fb{4, 3};

    check_colour(fb.get(0, 0), 0, 0, 0, 255);
    check_colour(fb.get(3, 2), 0, 0, 0, 255);
}

TEST_CASE("framebuffer get out of bounds")
{
    const framebuffer fb{4, 3};

    check_colour(fb.get(4, 0), 255, 255, 255, 255);
    check_colour(fb.get(0, 3), 255, 255, 255, 255);
    check_colour(fb.get(99, 99), 255, 255, 255, 255);
    check_colour(fb.get(-1, 0), 255, 255, 255, 255);
}

TEST_CASE("framebuffer set out of bounds ignored")
{
    framebuffer fb{4, 3};

    fb.set(4, 0, colour{1, 2, 3, 4});
    fb.set(0, 3, colour{1, 2, 3, 4});
    fb.set(99, 99, colour{1, 2, 3, 4});
    fb.set(-1, 0, colour{1, 2, 3, 4});

    check_colour(fb.get(3, 2), 0, 0, 0, 255);
}

TEST_CASE("framebuffer set and get")
{
    framebuffer fb{4, 3};

    fb.set(1, 2, colour{255, 0, 0, 255});
    check_colour(fb.get(1, 2), 255, 0, 0, 255);

    check_colour(fb.get(0, 0), 0, 0, 0, 255);
}

TEST_CASE("framebuffer arbitrary colour values")
{
    framebuffer fb{2, 2};

    fb.set(0, 0, colour{10, 20, 30, 40});
    check_colour(fb.get(0, 0), 10, 20, 30, 40);

    fb.set(1, 1, colour{200, 100, 50, 128});
    check_colour(fb.get(1, 1), 200, 100, 50, 128);
}

TEST_CASE("framebuffer overwrite")
{
    framebuffer fb{2, 2};

    fb.set(0, 0, colour{255, 255, 255, 255});
    fb.set(0, 0, colour{1, 2, 3, 4});
    check_colour(fb.get(0, 0), 1, 2, 3, 4);
}

TEST_CASE("framebuffer independent colours")
{
    framebuffer fb{8, 1};

    fb.set(1, 0, colour{99, 99, 99, 99});
    check_colour(fb.get(0, 0), 0, 0, 0, 255);
    check_colour(fb.get(2, 0), 0, 0, 0, 255);
}

TEST_CASE("framebuffer one colour render target")
{
    framebuffer fb{1, 1};

    check_colour(fb.get(0, 0), 0, 0, 0, 255);

    fb.set(0, 0, colour{0, 255, 0, 255});
    check_colour(fb.get(0, 0), 0, 255, 0, 255);
}

TEST_CASE("framebuffer clear")
{
    framebuffer fb{2, 2};
    fb.set(0, 0, colour{1, 2, 3, 4});

    fb.clear();
    check_colour(fb.get(0, 0), 0, 0, 0, 255);
    check_colour(fb.get(1, 1), 0, 0, 0, 255);

    fb.clear(white);
    check_colour(fb.get(0, 0), 255, 255, 255, 255);
    check_colour(fb.get(1, 1), 255, 255, 255, 255);
}