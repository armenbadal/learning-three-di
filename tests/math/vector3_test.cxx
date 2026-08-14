#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

import e3d;

#include <stdexcept>

using namespace e3d::math;

namespace {
    auto approx = [](float value) { return Catch::Approx(value).margin(1e-5f); };
}

constexpr vector3 test_constexpr_ctor{1.0F, 2.0F, 3.0F};

TEST_CASE("vector3 cross product basis")
{
    const vector3 x{1.0F, 0.0F, 0.0F};
    const vector3 y{0.0F, 1.0F, 0.0F};
    const vector3 z{0.0F, 0.0F, 1.0F};

    CHECK(cross(x, y) == z);
    CHECK(cross(y, z) == x);
    CHECK(cross(z, x) == y);
}

TEST_CASE("vector3 cross product anticommutative")
{
    const vector3 a{1.0F, 2.0F, 3.0F};
    const vector3 b{4.0F, 5.0F, 6.0F};

    CHECK(cross(a, b) == -cross(b, a));
}

TEST_CASE("vector3 cross product parallel is zero")
{
    const vector3 a{2.0F, 4.0F, 6.0F};
    const vector3 b{1.0F, 2.0F, 3.0F};
    const vector3 zero{0.0F, 0.0F, 0.0F};

    CHECK(cross(a, b) == zero);
}

TEST_CASE("vector3 cross product perpendicular")
{
    const vector3 a{1.0F, 2.0F, 3.0F};
    const vector3 b{4.0F, 5.0F, 6.0F};
    const vector3 c = cross(a, b);

    CHECK(dot(c, a) == approx(0.0F));
    CHECK(dot(c, b) == approx(0.0F));
}

TEST_CASE("vector3 equality")
{
    const vector3 a{1.0F, 2.0F, 3.0F};

    CHECK(a == vector3{1.0F, 2.0F, 3.0F});
    CHECK_FALSE(a != vector3{1.0F, 2.0F, 3.0F});

    CHECK_FALSE(a == vector3{1.0F + 1e-6F, 2.0F, 3.0F});
    CHECK(almost_equal(a, vector3{1.0F + 1e-6F, 2.0F, 3.0F}));

    CHECK_FALSE(a == vector3{1.0F + 1e-3F, 2.0F, 3.0F});
    CHECK(a != vector3{1.0F + 1e-3F, 2.0F, 3.0F});
    CHECK_FALSE(almost_equal(a, vector3{1.0F + 1e-3F, 2.0F, 3.0F}));
}

TEST_CASE("vector3 compound operators return the vector")
{
    vector3 v{1.0F, 2.0F, 3.0F};

    CHECK(&(v += vector3{1.0F, 1.0F, 1.0F}) == &v);
    CHECK(&(v -= vector3{1.0F, 1.0F, 1.0F}) == &v);
    CHECK(&(v *= 2.0F) == &v);
    CHECK(&(v /= 2.0F) == &v);
    CHECK(v == vector3{1.0F, 2.0F, 3.0F});
}

TEST_CASE("vector3 division by zero throws")
{
    vector3 v{1.0F, 2.0F, 3.0F};

    CHECK_THROWS_AS(v /= 0.0F, std::domain_error);
    CHECK_THROWS_AS(v / 0.0F, std::domain_error);
}
