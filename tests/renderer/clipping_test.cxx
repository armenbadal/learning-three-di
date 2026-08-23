#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "engine3d/renderer/software/clipping.hxx"
#include "engine3d/math/vector4.hxx"

#include <array>
#include <vector>

namespace math = e3d::math;
namespace renderer = e3d::renderer;

namespace {

bool same_vertex(const renderer::clip_vertex& a, const renderer::clip_vertex& b)
{
    return math::almost_equal(a.position, b.position);
}

float distance_to(renderer::clip_plane pl, const renderer::clip_vertex& vertex)
{
    const auto& p = vertex.position;

    switch( pl ) {
        case renderer::clip_plane::left:
            return p.w() + p.x();
        case renderer::clip_plane::right:
            return p.w() - p.x();
        case renderer::clip_plane::bottom:
            return p.w() + p.y();
        case renderer::clip_plane::top:
            return p.w() - p.y();
        case renderer::clip_plane::near:
            return p.w() + p.z();
        case renderer::clip_plane::far:
            return p.w() - p.z();
    }

    return -1.0F;
}

} // namespace

TEST_CASE("clip_vertex fields")
{
    const renderer::clip_vertex vertex{{1.0F, 2.0F, 3.0F, 4.0F}};

    CHECK(vertex.position.x() == 1.0F);
    CHECK(vertex.position.y() == 2.0F);
    CHECK(vertex.position.z() == 3.0F);
    CHECK(vertex.position.w() == 4.0F);
}

TEST_CASE("clip_vertex interpolation uses the same parameter for position and colour")
{
    const renderer::clip_vertex a{
        {-1.0F, 2.0F, 0.0F, 1.0F},
        {0.0F, 40.0F, 80.0F, 120.0F}
    };
    const renderer::clip_vertex b{
        {3.0F, -2.0F, 4.0F, 3.0F},
        {200.0F, 120.0F, 160.0F, 200.0F}
    };

    const auto result = renderer::interpolate(a, b, 0.25F);

    CHECK(math::almost_equal(result.position, math::vector4{0.0F, 1.0F, 1.0F, 1.5F}));
    CHECK(result.colour == e3d::graphics::colourf{50.0F, 60.0F, 100.0F, 140.0F});
}

TEST_CASE("clip_against_plane interpolates colour on generated vertices")
{
    const renderer::clip_vertex inside_a{{0.0F, 0.0F, 0.0F, 1.0F}, {0.0F, 0.0F, 0.0F}};
    const renderer::clip_vertex outside{{2.0F, 0.0F, 0.0F, 1.0F}, {200.0F, 100.0F, 50.0F}};
    const renderer::clip_vertex inside_b{{0.0F, 1.0F, 0.0F, 1.0F}, {0.0F, 200.0F, 250.0F}};
    const std::array input{inside_a, outside, inside_b};

    const auto output = renderer::clip_against_plane(input, renderer::clip_plane::right);

    REQUIRE(output.size() == 4);
    CHECK(math::almost_equal(output[1].position, math::vector4{1.0F, 0.0F, 0.0F, 1.0F}));
    CHECK(output[1].colour == e3d::graphics::colourf{100.0F, 50.0F, 25.0F});
    CHECK(math::almost_equal(output[2].position, math::vector4{1.0F, 0.5F, 0.0F, 1.0F}));
    CHECK(output[2].colour == e3d::graphics::colourf{100.0F, 150.0F, 150.0F});
}

TEST_CASE("clip_against_plane returns an empty polygon for empty input")
{
    const std::vector<renderer::clip_vertex> input;

    CHECK(renderer::clip_against_plane(input, renderer::clip_plane::right).empty());
}

TEST_CASE("clip_against_plane preserves a polygon entirely inside")
{
    const std::vector<renderer::clip_vertex> input{
        {{-0.5F, -0.5F, 0.0F, 1.0F}},
        {{0.5F, -0.5F, 0.0F, 1.0F}},
        {{0.0F, 0.5F, 0.0F, 1.0F}},
    };

    const auto output = renderer::clip_against_plane(input, renderer::clip_plane::right);

    REQUIRE(output.size() == input.size());
    for( std::size_t i = 0; i < input.size(); ++i )
        CHECK(same_vertex(output[i], input[i]));
}

TEST_CASE("clip_against_plane removes a polygon entirely outside")
{
    const std::vector<renderer::clip_vertex> input{
        {{2.0F, -0.5F, 0.0F, 1.0F}},
        {{3.0F, 0.0F, 0.0F, 1.0F}},
        {{2.0F, 0.5F, 0.0F, 1.0F}},
    };

    CHECK(renderer::clip_against_plane(input, renderer::clip_plane::right).empty());
}

TEST_CASE("clip_against_plane clips against every homogeneous clip plane")
{
    struct plane_case {
        renderer::clip_plane pl;
        math::vector4 outside;
    };

    const std::array cases{
        plane_case{renderer::clip_plane::left, {-3.0F, 0.0F, 0.0F, 2.0F}},
        plane_case{renderer::clip_plane::right, {3.0F, 0.0F, 0.0F, 2.0F}},
        plane_case{renderer::clip_plane::bottom, {0.0F, -3.0F, 0.0F, 2.0F}},
        plane_case{renderer::clip_plane::top, {0.0F, 3.0F, 0.0F, 2.0F}},
        plane_case{renderer::clip_plane::near, {0.0F, 0.0F, -3.0F, 2.0F}},
        plane_case{renderer::clip_plane::far, {0.0F, 0.0F, 3.0F, 2.0F}},
    };

    const renderer::clip_vertex a{{-0.5F, -0.5F, -0.5F, 2.0F}};
    const renderer::clip_vertex b{{0.5F, 0.5F, 0.5F, 2.0F}};

    for( const auto& test : cases ) {
        const std::array input{a, renderer::clip_vertex{test.outside}, b};
        const auto output = renderer::clip_against_plane(input, test.pl);

        REQUIRE(output.size() == 4);
        CHECK(same_vertex(output.front(), a));
        CHECK(same_vertex(output.back(), b));
        CHECK(distance_to(test.pl, output[1]) == Catch::Approx(0.0F));
        CHECK(distance_to(test.pl, output[2]) == Catch::Approx(0.0F));
        for( const auto& vertex : output )
            CHECK(distance_to(test.pl, vertex) >= 0.0F);
    }
}

TEST_CASE("clip_against_plane does not duplicate a vertex on the plane")
{
    const renderer::clip_vertex boundary{{1.0F, 0.0F, 0.0F, 1.0F}};
    const renderer::clip_vertex outside{{2.0F, 0.0F, 0.0F, 1.0F}};
    const renderer::clip_vertex inside{{0.0F, 1.0F, 0.0F, 1.0F}};
    const std::array input{boundary, outside, inside};

    const auto output = renderer::clip_against_plane(input, renderer::clip_plane::right);

    REQUIRE(output.size() == 3);
    CHECK(same_vertex(output[0], boundary));
    CHECK(math::almost_equal(output[1].position, math::vector4{1.0F, 0.5F, 0.0F, 1.0F}));
    CHECK(same_vertex(output[2], inside));
}
