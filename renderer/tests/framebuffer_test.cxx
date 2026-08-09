#include <catch2/catch_test_macros.hpp>

#include "framebuffer.hxx"

#include <limits>
#include <stdexcept>

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

    check_colour(fb.at(0, 0), 0, 0, 0, 255);
    check_colour(fb.at(3, 2), 0, 0, 0, 255);
}

TEST_CASE("framebuffer at throws on out of bounds")
{
    framebuffer fb{4, 3};

    CHECK_THROWS_AS(fb.at(4, 0), std::out_of_range);
    CHECK_THROWS_AS(fb.at(0, 3), std::out_of_range);
    CHECK_THROWS_AS(fb.at(99, 99), std::out_of_range);
    CHECK_THROWS_AS(fb.at(std::numeric_limits<std::size_t>::max(), 0), std::out_of_range);
}

TEST_CASE("framebuffer at read and write")
{
    framebuffer fb{4, 3};

    fb.at(1, 2) = colour{255, 0, 0, 255};
    check_colour(fb.at(1, 2), 255, 0, 0, 255);
    check_colour(fb.at(0, 0), 0, 0, 0, 255);

    CHECK_THROWS_AS((fb.at(4, 0) = colour{1, 2, 3, 4}), std::out_of_range);
}

TEST_CASE("framebuffer operator() reads and writes in bounds")
{
    framebuffer fb{2, 2};

    fb(0, 0) = colour{10, 20, 30, 40};
    check_colour(fb(0, 0), 10, 20, 30, 40);

    fb(1, 1) = colour{200, 100, 50, 128};
    check_colour(fb(1, 1), 200, 100, 50, 128);
}

TEST_CASE("framebuffer set_clipped ignores out of bounds")
{
    framebuffer fb{4, 3};

    fb.set_clipped(4, 0, colour{1, 2, 3, 4});
    fb.set_clipped(0, 3, colour{1, 2, 3, 4});
    fb.set_clipped(99, 99, colour{1, 2, 3, 4});
    fb.set_clipped(-1, 0, colour{1, 2, 3, 4});
    fb.set_clipped(0, -1, colour{1, 2, 3, 4});

    check_colour(fb.at(3, 2), 0, 0, 0, 255);
}

TEST_CASE("framebuffer set_clipped writes in bounds")
{
    framebuffer fb{4, 3};

    fb.set_clipped(1, 2, colour{255, 0, 0, 255});
    check_colour(fb.at(1, 2), 255, 0, 0, 255);
    check_colour(fb.at(0, 0), 0, 0, 0, 255);
}

TEST_CASE("framebuffer overwrite")
{
    framebuffer fb{2, 2};

    fb(0, 0) = colour{255, 255, 255, 255};
    fb(0, 0) = colour{1, 2, 3, 4};
    check_colour(fb.at(0, 0), 1, 2, 3, 4);
}

TEST_CASE("framebuffer independent colours")
{
    framebuffer fb{8, 1};

    fb(1, 0) = colour{99, 99, 99, 99};
    check_colour(fb.at(0, 0), 0, 0, 0, 255);
    check_colour(fb.at(2, 0), 0, 0, 0, 255);
}

TEST_CASE("framebuffer one colour render target")
{
    framebuffer fb{1, 1};

    check_colour(fb.at(0, 0), 0, 0, 0, 255);

    fb.at(0, 0) = colour{0, 255, 0, 255};
    check_colour(fb.at(0, 0), 0, 255, 0, 255);
}

TEST_CASE("framebuffer clear")
{
    framebuffer fb{2, 2};
    fb.at(0, 0) = colour{1, 2, 3, 4};

    fb.clear();
    check_colour(fb.at(0, 0), 0, 0, 0, 255);
    check_colour(fb.at(1, 1), 0, 0, 0, 255);

    fb.clear(white);
    check_colour(fb.at(0, 0), 255, 255, 255, 255);
    check_colour(fb.at(1, 1), 255, 255, 255, 255);
}

TEST_CASE("framebuffer constructor guards against overflow")
{
    CHECK_THROWS_AS((framebuffer{std::numeric_limits<std::size_t>::max(), 2}), std::invalid_argument);
    CHECK_THROWS_AS((framebuffer{std::numeric_limits<std::size_t>::max(), 0}), std::invalid_argument);
}
