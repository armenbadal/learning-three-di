#include <catch2/catch_test_macros.hpp>

#include "colour.hxx"

using namespace renderer;

namespace {
    constexpr colour test_constexpr_colour{255, 128, 0, 64};
}

static_assert(test_constexpr_colour.r() == 255);
static_assert(test_constexpr_colour.g() == 128);
static_assert(test_constexpr_colour.b() == 0);
static_assert(test_constexpr_colour.a() == 64);

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