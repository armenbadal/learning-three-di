#include <catch2/catch_test_macros.hpp>

#include "engine3d/graphics/colour.hxx"

namespace graphics = e3d::graphics;
using namespace e3d::graphics;

namespace {
    constexpr colour test_constexpr_colour{255, 128, 0, 64};
}

static_assert(test_constexpr_colour.r() == 255);
static_assert(test_constexpr_colour.g() == 128);
static_assert(test_constexpr_colour.b() == 0);
static_assert(test_constexpr_colour.a() == 64);

static_assert((graphics::colour{100, 50, 25, 10} + graphics::colour{100, 50, 25, 10}) == graphics::colour{200, 100, 50, 20});
static_assert((graphics::colour{200, 200, 200, 200} + graphics::colour{100, 100, 100, 100}) == graphics::colour{255, 255, 255, 255});
static_assert((graphics::colour{255, 128, 64, 32} * 0.5F) == graphics::colour{128, 64, 32, 16});

TEST_CASE("colour components")
{
    const colour p{10, 20, 30};

    CHECK(p.r() == 10);
    CHECK(p.g() == 20);
    CHECK(p.b() == 30);
}

TEST_CASE("colour default alpha")
{
    const colour p{1, 2, 3};

    CHECK(p.a() == 255);
}

TEST_CASE("colour explicit alpha")
{
    const colour p{1, 2, 3, 128};

    CHECK(p.a() == 128);
}

TEST_CASE("colour equality")
{
    const colour a{10, 20, 30, 40};

    CHECK(a == colour{10, 20, 30, 40});
    CHECK_FALSE(a != colour{10, 20, 30, 40});

    CHECK_FALSE(a == colour{11, 20, 30, 40});
    CHECK(a != colour{11, 20, 30, 40});

    CHECK_FALSE(a == colour{10, 20, 30, 41});
    CHECK(a != colour{10, 20, 30, 41});

    CHECK(white == colour{255, 255, 255, 255});
    CHECK(black == colour{0, 0, 0, 255});
    CHECK_FALSE(white == black);
}

TEST_CASE("colour addition")
{
    CHECK((colour{10, 20, 30, 40} + colour{100, 50, 5, 10}) == colour{110, 70, 35, 50});
    CHECK((black + black) == black);
}

TEST_CASE("colour addition saturates")
{
    CHECK((colour{200, 0, 0, 0} + colour{100, 50, 50, 50}) == colour{255, 50, 50, 50});
    CHECK((colour{255, 255, 255, 255} + colour{1, 1, 1, 1}) == colour{255, 255, 255, 255});
}

TEST_CASE("colour multiplication")
{
    CHECK((white * 0.5F) == colour{128, 128, 128, 128});
    CHECK((colour{255, 128, 64, 32} * 0.5F) == colour{128, 64, 32, 16});
    CHECK((colour{10, 20, 30, 40} * 0.0F) == colour{0, 0, 0, 0});
    CHECK((colour{10, 20, 30, 40} * 1.0F) == colour{10, 20, 30, 40});
}

TEST_CASE("colour multiplication clamps and rounds")
{
    CHECK((white * 2.0F) == colour{255, 255, 255, 255});
    CHECK((colour{1, 2, 3, 4} * -1.0F) == colour{0, 0, 0, 0});
}
