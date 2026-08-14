#include <catch2/catch_test_macros.hpp>

import e3d;

using namespace e3d::geometry;
using namespace e3d::graphics;

TEST_CASE("vertex2d fields")
{
    const vertex2d v{{1.0F, 2.0F}, {10, 20, 30, 40}};

    CHECK(v.position.x() == 1.0F);
    CHECK(v.position.y() == 2.0F);
    CHECK(v.colour == colour{10, 20, 30, 40});
}

TEST_CASE("vertex3d stores a model-space position")
{
    const vertex3d vertex{{1.0F, 2.0F, 3.0F}, {10, 20, 30, 40}};

    CHECK(vertex.position.x() == 1.0F);
    CHECK(vertex.position.y() == 2.0F);
    CHECK(vertex.position.z() == 3.0F);
    CHECK(vertex.colour == colour{10, 20, 30, 40});
}

TEST_CASE("vertex3d position enters the pipeline with homogeneous W equal to one")
{
    const vertex3d vertex{{1.0F, 2.0F, 3.0F}};
    const e3d::math::vector4 homogeneous{vertex.position, 1.0F};

    CHECK(homogeneous.x() == 1.0F);
    CHECK(homogeneous.y() == 2.0F);
    CHECK(homogeneous.z() == 3.0F);
    CHECK(homogeneous.w() == 1.0F);
}
