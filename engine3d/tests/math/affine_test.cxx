#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "engine3d/math/affine.hxx"

#include <numbers>

using namespace engine3d::math;

namespace {
    constexpr float pi = std::numbers::pi_v<float>;

    auto approx = [](float value) { return Catch::Approx(value).margin(1e-5f); };
}

TEST_CASE("transform identity")
{
    const transform t{{0.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, 0.0F},
                      {1.0F, 1.0F, 1.0F}};

    CHECK(t.model_matrix() == matrix4x4{});
}

TEST_CASE("transform translation")
{
    const transform t{{1.0F, 2.0F, 3.0F},
                      {0.0F, 0.0F, 0.0F},
                      {1.0F, 1.0F, 1.0F}};
    const vector4 p{0.0F, 0.0F, 0.0F, 1.0F};

    const vector4 result = t.model_matrix() * p;
    CHECK(result.x() == approx(1.0F));
    CHECK(result.y() == approx(2.0F));
    CHECK(result.z() == approx(3.0F));
}

TEST_CASE("transform scaling")
{
    const transform t{{0.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, 0.0F},
                      {2.0F, 3.0F, 4.0F}};
    const vector4 p{1.0F, 1.0F, 1.0F, 1.0F};

    const vector4 result = t.model_matrix() * p;
    CHECK(result.x() == approx(2.0F));
    CHECK(result.y() == approx(3.0F));
    CHECK(result.z() == approx(4.0F));
}

TEST_CASE("transform rotation")
{
    const transform t{{0.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, pi / 2.0F},
                      {1.0F, 1.0F, 1.0F}};
    const vector4 p{1.0F, 0.0F, 0.0F, 1.0F};

    const vector4 result = t.model_matrix() * p;
    CHECK(result.x() == approx(0.0F));
    CHECK(result.y() == approx(1.0F));
    CHECK(result.z() == approx(0.0F));
}

TEST_CASE("transform trs order")
{
    const transform t{{1.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, 0.0F},
                      {2.0F, 2.0F, 2.0F}};
    const vector4 p{1.0F, 1.0F, 1.0F, 1.0F};

    const vector4 result = t.model_matrix() * p;
    CHECK(result.x() == approx(3.0F));
    CHECK(result.y() == approx(2.0F));
    CHECK(result.z() == approx(2.0F));

    const vector4 direction{1.0F, 1.0F, 1.0F, 0.0F};
    const vector4 dr = t.model_matrix() * direction;
    CHECK(dr.x() == approx(2.0F));
    CHECK(dr.y() == approx(2.0F));
    CHECK(dr.z() == approx(2.0F));
}
