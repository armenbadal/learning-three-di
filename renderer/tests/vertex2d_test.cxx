#include <catch2/catch_test_macros.hpp>

#include "vertex2d.hxx"

using namespace renderer;

TEST_CASE("vertex2d fields")
{
    const vertex2d v{{1.0F, 2.0F}, {10, 20, 30, 40}};

    CHECK(v._position.x() == 1.0F);
    CHECK(v._position.y() == 2.0F);
    CHECK(v._colour == colour{10, 20, 30, 40});
}