#include <catch2/catch_test_macros.hpp>

#include "engine3d/geometry/vertex2d.hxx"

using namespace engine3d::renderer;

TEST_CASE("vertex2d fields")
{
    const vertex2d v{{1.0F, 2.0F}, {10, 20, 30, 40}};

    CHECK(v.position.x() == 1.0F);
    CHECK(v.position.y() == 2.0F);
    CHECK(v.colour == colour{10, 20, 30, 40});
}
