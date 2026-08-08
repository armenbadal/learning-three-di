#include <catch2/catch_test_macros.hpp>

#include "framebuffer.hxx"

using namespace renderer;

namespace {
    void check_pixel(const pixel& p, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
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

    check_pixel(fb.get(0, 0), 0, 0, 0, 255);
    check_pixel(fb.get(3, 2), 0, 0, 0, 255);
}

TEST_CASE("framebuffer get out of bounds")
{
    const framebuffer fb{4, 3};

    check_pixel(fb.get(4, 0), 255, 255, 255, 255);
    check_pixel(fb.get(0, 3), 255, 255, 255, 255);
    check_pixel(fb.get(99, 99), 255, 255, 255, 255);
    check_pixel(fb.get(-1, 0), 255, 255, 255, 255);
}

TEST_CASE("framebuffer set out of bounds ignored")
{
    framebuffer fb{4, 3};

    fb.set(4, 0, pixel{1, 2, 3, 4});
    fb.set(0, 3, pixel{1, 2, 3, 4});
    fb.set(99, 99, pixel{1, 2, 3, 4});
    fb.set(-1, 0, pixel{1, 2, 3, 4});

    check_pixel(fb.get(3, 2), 0, 0, 0, 255);
}

TEST_CASE("framebuffer set and get")
{
    framebuffer fb{4, 3};

    fb.set(1, 2, pixel{255, 0, 0, 255});
    check_pixel(fb.get(1, 2), 255, 0, 0, 255);

    check_pixel(fb.get(0, 0), 0, 0, 0, 255);
}

TEST_CASE("framebuffer arbitrary pixel values")
{
    framebuffer fb{2, 2};

    fb.set(0, 0, pixel{10, 20, 30, 40});
    check_pixel(fb.get(0, 0), 10, 20, 30, 40);

    fb.set(1, 1, pixel{200, 100, 50, 128});
    check_pixel(fb.get(1, 1), 200, 100, 50, 128);
}

TEST_CASE("framebuffer overwrite")
{
    framebuffer fb{2, 2};

    fb.set(0, 0, pixel{255, 255, 255, 255});
    fb.set(0, 0, pixel{1, 2, 3, 4});
    check_pixel(fb.get(0, 0), 1, 2, 3, 4);
}

TEST_CASE("framebuffer independent pixels")
{
    framebuffer fb{8, 1};

    fb.set(1, 0, pixel{99, 99, 99, 99});
    check_pixel(fb.get(0, 0), 0, 0, 0, 255);
    check_pixel(fb.get(2, 0), 0, 0, 0, 255);
}

TEST_CASE("framebuffer one pixel render target")
{
    framebuffer fb{1, 1};

    check_pixel(fb.get(0, 0), 0, 0, 0, 255);

    fb.set(0, 0, pixel{0, 255, 0, 255});
    check_pixel(fb.get(0, 0), 0, 255, 0, 255);
}