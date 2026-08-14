#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>

import e3d;

namespace math = e3d::math;
namespace renderer = e3d::renderer;

namespace {

auto approx(float value)
{
    return Catch::Approx(value).margin(1e-5F);
}

} // namespace

TEST_CASE("viewport_transform maps the NDC centre to the viewport centre")
{
    const auto result = renderer::viewport_transform(
        math::vector3{0.0F, 0.0F, 0.0F}, 800.0F, 600.0F);

    CHECK(result.position.x() == approx(400.0F));
    CHECK(result.position.y() == approx(300.0F));
    CHECK(result.depth == approx(0.5F));
}

TEST_CASE("viewport_transform maps NDC corners and flips the Y axis")
{
    const auto top_left = renderer::viewport_transform(
        math::vector3{-1.0F, 1.0F, -1.0F}, 800.0F, 600.0F);
    const auto bottom_right = renderer::viewport_transform(
        math::vector3{1.0F, -1.0F, 1.0F}, 800.0F, 600.0F);

    CHECK(top_left.position.x() == approx(0.0F));
    CHECK(top_left.position.y() == approx(0.0F));
    CHECK(top_left.depth == approx(0.0F));

    CHECK(bottom_right.position.x() == approx(800.0F));
    CHECK(bottom_right.position.y() == approx(600.0F));
    CHECK(bottom_right.depth == approx(1.0F));
}

TEST_CASE("viewport_transform scales axes independently")
{
    const auto result = renderer::viewport_transform(
        math::vector3{0.5F, -0.5F, 0.25F}, 320.0F, 200.0F);

    CHECK(result.position.x() == approx(240.0F));
    CHECK(result.position.y() == approx(150.0F));
    CHECK(result.depth == approx(0.625F));
}

TEST_CASE("perspective_divide preserves coordinates when W is one")
{
    const auto result = renderer::perspective_divide(
        math::vector4{0.25F, -0.5F, 0.75F, 1.0F});

    CHECK(result.x() == approx(0.25F));
    CHECK(result.y() == approx(-0.5F));
    CHECK(result.z() == approx(0.75F));
}

TEST_CASE("perspective_divide divides every spatial coordinate by W")
{
    const auto result = renderer::perspective_divide(
        math::vector4{2.0F, 4.0F, -6.0F, 2.0F});

    CHECK(result.x() == approx(1.0F));
    CHECK(result.y() == approx(2.0F));
    CHECK(result.z() == approx(-3.0F));
}

TEST_CASE("perspective_divide handles negative W")
{
    const auto result = renderer::perspective_divide(
        math::vector4{2.0F, -4.0F, 6.0F, -2.0F});

    CHECK(result.x() == approx(-1.0F));
    CHECK(result.y() == approx(2.0F));
    CHECK(result.z() == approx(-3.0F));
}

TEST_CASE("perspective_divide is invariant under homogeneous scaling")
{
    const auto original = renderer::perspective_divide(
        math::vector4{1.0F, -2.0F, 3.0F, 2.0F});
    const auto scaled = renderer::perspective_divide(
        math::vector4{4.0F, -8.0F, 12.0F, 8.0F});

    CHECK(math::almost_equal(original, scaled));
}

TEST_CASE("perspective divide and viewport transform map the NDC bounds")
{
    constexpr float width = 800.0F;
    constexpr float height = 600.0F;

    struct mapping {
        math::vector4 clip;
        math::vector3 ndc;
        math::vector3 screen;
    };

    const std::array mappings{
        mapping{{-2.0F, 2.0F, -2.0F, 2.0F}, {-1.0F, 1.0F, -1.0F}, {0.0F, 0.0F, 0.0F}},
        mapping{{0.0F, 0.0F, 0.0F, 2.0F}, {0.0F, 0.0F, 0.0F}, {width / 2.0F, height / 2.0F, 0.5F}},
        mapping{{2.0F, -2.0F, 2.0F, 2.0F}, {1.0F, -1.0F, 1.0F}, {width, height, 1.0F}},
    };

    for( const auto& expected : mappings ) {
        const auto ndc = renderer::perspective_divide(expected.clip);
        CHECK(math::almost_equal(ndc, expected.ndc));

        const auto screen = renderer::viewport_transform(ndc, width, height);
        CHECK(screen.position.x() == approx(expected.screen.x()));
        CHECK(screen.position.y() == approx(expected.screen.y()));
        CHECK(screen.depth == approx(expected.screen.z()));
    }
}
