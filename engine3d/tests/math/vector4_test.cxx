#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <engine3d/math/vector4.hxx>

#include <stdexcept>

using namespace engine3d::math;

namespace {
    auto approx = [](float value) { return Catch::Approx(value).margin(1e-5f); };
}

constexpr vector4 test_constexpr_ctor{1.0F, 2.0F, 3.0F, 4.0F};

TEST_CASE("vector4 add")
{
    const vector4 v0{1.0F, 2.0F, 3.0F, 4.0F};
    const vector4 v1{5.0F, 6.0F, 7.0F, 8.0F};

    CHECK(v0 + v1 == vector4{6.0F, 8.0F, 10.0F, 12.0F});
}

TEST_CASE("vector4 length")
{
    const vector4 v0{1.0F, 2.0F, 2.0F, 4.0F};

    CHECK(v0.length() == approx(5.0F));
}

TEST_CASE("vector4 normalize")
{
    vector4 v0{3.0F, 0.0F, 0.0F, 4.0F};
    v0.normalize();

    CHECK(v0.x() == approx(0.6F));
    CHECK(v0.y() == approx(0.0F));
    CHECK(v0.z() == approx(0.0F));
    CHECK(v0.w() == approx(0.8F));
}

TEST_CASE("vector4 dot product")
{
    const vector4 v0{1.0F, 2.0F, 3.0F, 4.0F};
    const vector4 v1{5.0F, 6.0F, 7.0F, 8.0F};

    CHECK(v0.dot_product(v1) == approx(70.0F));
}

TEST_CASE("vector4 equality")
{
    const vector4 a{1.0F, 2.0F, 3.0F, 4.0F};

    CHECK(a == vector4{1.0F, 2.0F, 3.0F, 4.0F});
    CHECK_FALSE(a != vector4{1.0F, 2.0F, 3.0F, 4.0F});

    CHECK_FALSE(a == vector4{1.0F + 1e-6F, 2.0F, 3.0F, 4.0F});
    CHECK(almost_equal(a, vector4{1.0F + 1e-6F, 2.0F, 3.0F, 4.0F}));

    CHECK_FALSE(a == vector4{1.0F + 1e-3F, 2.0F, 3.0F, 4.0F});
    CHECK(a != vector4{1.0F + 1e-3F, 2.0F, 3.0F, 4.0F});
    CHECK_FALSE(almost_equal(a, vector4{1.0F + 1e-3F, 2.0F, 3.0F, 4.0F}));
}

TEST_CASE("vector4 compound operators return the vector")
{
    vector4 v{1.0F, 2.0F, 3.0F, 4.0F};

    CHECK(&(v += vector4{1.0F, 1.0F, 1.0F, 1.0F}) == &v);
    CHECK(&(v -= vector4{1.0F, 1.0F, 1.0F, 1.0F}) == &v);
    CHECK(&(v *= 2.0F) == &v);
    CHECK(&(v /= 2.0F) == &v);
    CHECK(v == vector4{1.0F, 2.0F, 3.0F, 4.0F});
}

TEST_CASE("vector4 division by zero throws")
{
    vector4 v{1.0F, 2.0F, 3.0F, 4.0F};

    CHECK_THROWS_AS(v /= 0.0F, std::domain_error);
    CHECK_THROWS_AS(v / 0.0F, std::domain_error);
}
