#include <catch2/catch_test_macros.hpp>

#include "pixel.hxx"

using namespace renderer;

namespace {
    constexpr pixel test_constexpr_pixel{255, 128, 0, 64};
}

static_assert(test_constexpr_pixel.r() == 255);
static_assert(test_constexpr_pixel.g() == 128);
static_assert(test_constexpr_pixel.b() == 0);
static_assert(test_constexpr_pixel.a() == 64);

TEST_CASE("pixel components")
{
    const pixel p{10, 20, 30};

    CHECK(p.r() == 10);
    CHECK(p.g() == 20);
    CHECK(p.b() == 30);
}

TEST_CASE("pixel default alpha")
{
    const pixel p{1, 2, 3};

    CHECK(p.a() == 255);
}

TEST_CASE("pixel explicit alpha")
{
    const pixel p{1, 2, 3, 128};

    CHECK(p.a() == 128);
}