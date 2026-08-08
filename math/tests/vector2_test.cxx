#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "vector2.hxx"

using namespace math;

namespace {
    auto approx = [](float value) { return Catch::Approx(value).margin(1e-5f); };
}

constexpr vector2 test_constexpr_ctor{3.0F, 4.0F};

TEST_CASE("vector2 add")
{
    const vector2 v0{1.0F, 2.0F};
    const vector2 v1{3.0F, 4.0F};
    const vector2 v2 = v0 + v1;
    const vector2 v3{4.0F, 6.0F};

    CHECK(v2 == v3);
}

TEST_CASE("vector2 length")
{
    const vector2 v0{3.0F, 4.0F};

    CHECK(v0.length() == approx(5.0F));
}

TEST_CASE("vector2 normalize")
{
    vector2 v0{3.0F, 4.0F};
    v0.normalize();

    CHECK(v0.x() == approx(0.6F));
    CHECK(v0.y() == approx(0.8F));
}

TEST_CASE("vector2 normalize zero")
{
    vector2 v0{0.0F, 0.0F};
    v0.normalize();
    CHECK(v0.x() == approx(0.0F));
    CHECK(v0.y() == approx(0.0F));

    const vector2 v1 = v0.normalized();
    CHECK(v1.x() == approx(0.0F));
    CHECK(v1.y() == approx(0.0F));
}

TEST_CASE("vector2 cross product")
{
    const vector2 x{1.0F, 0.0F};
    const vector2 y{0.0F, 1.0F};

    CHECK(x.cross_product(y) == approx(1.0F));
    CHECK(y.cross_product(x) == approx(-1.0F));
    CHECK(x.cross_product(x) == approx(0.0F));

    const vector2 a{3.0F, 4.0F};
    const vector2 b{5.0F, 6.0F};

    CHECK(a.cross_product(b) == approx(-2.0F));
    CHECK(a.cross_product(b) == -b.cross_product(a));
}

TEST_CASE("vector2 equality")
{
    const vector2 a{1.0F, 2.0F};

    CHECK(a == vector2{1.0F, 2.0F});
    CHECK_FALSE(a != vector2{1.0F, 2.0F});

    CHECK(a == vector2{1.0F + 1e-6F, 2.0F});
    CHECK_FALSE(a != vector2{1.0F + 1e-6F, 2.0F});

    CHECK_FALSE(a == vector2{1.0F + 1e-3F, 2.0F});
    CHECK(a != vector2{1.0F + 1e-3F, 2.0F});
}
