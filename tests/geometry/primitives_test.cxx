#include <catch2/catch_test_macros.hpp>

#include "engine3d/geometry/primitives.hxx"

#include <algorithm>
#include <array>
#include <cstdint>

namespace e3d::geometry::primitives {

TEST_CASE("cube is centred and has twelve triangles")
{
    const auto mesh = cube(2.0F);
    const auto vertices = mesh.vertices();
    const auto indices = mesh.indices();

    REQUIRE(vertices.size() == 8);
    CHECK(mesh.triangle_count() == 12);
    CHECK(indices.size() == 36);
    CHECK(vertices[0].position == math::vector3{-1.0F, -1.0F, -1.0F});
    CHECK(vertices[6].position == math::vector3{1.0F, 1.0F, 1.0F});
}

TEST_CASE("cube assigns the requested colours to its vertices")
{
    const std::array colors{
        graphics::colour{1, 2, 3},
        graphics::colour{4, 5, 6},
        graphics::colour{7, 8, 9},
        graphics::colour{10, 11, 12},
        graphics::colour{13, 14, 15},
        graphics::colour{16, 17, 18},
        graphics::colour{19, 20, 21},
        graphics::colour{22, 23, 24},
    };

    const auto mesh = cube(1.0F, colors);
    const auto vertices = mesh.vertices();

    for( std::size_t i = 0; i < colors.size(); ++i )
        CHECK(vertices[i].colour == colors[i]);
}

TEST_CASE("plane lies on the XZ plane with upward winding")
{
    const auto mesh = plane(4.0F, 2.0F);
    const auto vertices = mesh.vertices();
    const auto indices = mesh.indices();

    REQUIRE(vertices.size() == 4);
    CHECK(mesh.triangle_count() == 2);
    CHECK(std::ranges::equal(indices, std::array<std::uint32_t, 6>{0, 2, 1, 0, 3, 2}));
    CHECK(vertices[0].position == math::vector3{-2.0F, 0.0F, -1.0F});
    CHECK(vertices[2].position == math::vector3{2.0F, 0.0F, 1.0F});
}

} // namespace e3d::geometry::primitives
