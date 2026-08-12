#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "engine3d/geometry/polygon.hxx"
#include "engine3d/math/vector4.hxx"

#include <array>
#include <vector>

namespace geometry = engine3d::geometry;
namespace math = engine3d::math;

namespace {

bool same_vertex(const geometry::clip_vertex& a, const geometry::clip_vertex& b)
{
    return math::almost_equal(a.position, b.position);
}

float distance_to(geometry::clip_plane pl, const geometry::clip_vertex& vertex)
{
    const auto& p = vertex.position;

    switch( pl ) {
        case geometry::clip_plane::left:
            return p.w() + p.x();
        case geometry::clip_plane::right:
            return p.w() - p.x();
        case geometry::clip_plane::bottom:
            return p.w() + p.y();
        case geometry::clip_plane::top:
            return p.w() - p.y();
        case geometry::clip_plane::near:
            return p.w() + p.z();
        case geometry::clip_plane::far:
            return p.w() - p.z();
    }

    return -1.0F;
}

} // namespace

TEST_CASE("clip_against_plane returns an empty polygon for empty input")
{
    const std::vector<geometry::clip_vertex> input;

    CHECK(geometry::clip_against_plane(input, geometry::clip_plane::right).empty());
}

TEST_CASE("clip_against_plane preserves a polygon entirely inside")
{
    const std::vector<geometry::clip_vertex> input{
        {{-0.5F, -0.5F, 0.0F, 1.0F}},
        {{0.5F, -0.5F, 0.0F, 1.0F}},
        {{0.0F, 0.5F, 0.0F, 1.0F}},
    };

    const auto output = geometry::clip_against_plane(input, geometry::clip_plane::right);

    REQUIRE(output.size() == input.size());
    for( std::size_t i = 0; i < input.size(); ++i )
        CHECK(same_vertex(output[i], input[i]));
}

TEST_CASE("clip_against_plane removes a polygon entirely outside")
{
    const std::vector<geometry::clip_vertex> input{
        {{2.0F, -0.5F, 0.0F, 1.0F}},
        {{3.0F, 0.0F, 0.0F, 1.0F}},
        {{2.0F, 0.5F, 0.0F, 1.0F}},
    };

    CHECK(geometry::clip_against_plane(input, geometry::clip_plane::right).empty());
}

TEST_CASE("clip_against_plane clips against every homogeneous clip plane")
{
    struct plane_case {
        geometry::clip_plane pl;
        math::vector4 outside;
    };

    const std::array cases{
        plane_case{geometry::clip_plane::left, {-3.0F, 0.0F, 0.0F, 2.0F}},
        plane_case{geometry::clip_plane::right, {3.0F, 0.0F, 0.0F, 2.0F}},
        plane_case{geometry::clip_plane::bottom, {0.0F, -3.0F, 0.0F, 2.0F}},
        plane_case{geometry::clip_plane::top, {0.0F, 3.0F, 0.0F, 2.0F}},
        plane_case{geometry::clip_plane::near, {0.0F, 0.0F, -3.0F, 2.0F}},
        plane_case{geometry::clip_plane::far, {0.0F, 0.0F, 3.0F, 2.0F}},
    };

    const geometry::clip_vertex a{{-0.5F, -0.5F, -0.5F, 2.0F}};
    const geometry::clip_vertex b{{0.5F, 0.5F, 0.5F, 2.0F}};

    for( const auto& test : cases ) {
        const std::array input{a, geometry::clip_vertex{test.outside}, b};
        const auto output = geometry::clip_against_plane(input, test.pl);

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
    const geometry::clip_vertex boundary{{1.0F, 0.0F, 0.0F, 1.0F}};
    const geometry::clip_vertex outside{{2.0F, 0.0F, 0.0F, 1.0F}};
    const geometry::clip_vertex inside{{0.0F, 1.0F, 0.0F, 1.0F}};
    const std::array input{boundary, outside, inside};

    const auto output = geometry::clip_against_plane(input, geometry::clip_plane::right);

    REQUIRE(output.size() == 3);
    CHECK(same_vertex(output[0], boundary));
    CHECK(math::almost_equal(output[1].position, math::vector4{1.0F, 0.5F, 0.0F, 1.0F}));
    CHECK(same_vertex(output[2], inside));
}
