#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "engine3d/camera/camera.hxx"
#include "engine3d/math/transform.hxx"

using namespace engine3d::math;
using namespace engine3d::renderer;

namespace {
    auto approx = [](float value) { return Catch::Approx(value).margin(1e-5f); };
}

TEST_CASE("camera constructor compiles with position, target, up")
{
    const camera cam({1.0F, 2.0F, 3.0F}, {4.0F, 5.0F, 6.0F}, {0.0F, 1.0F, 0.0F});
    CHECK(true);
}

TEST_CASE("camera default up is Y-axis")
{
    const camera cam({0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, -1.0F}, {0.0F, 1.0F, 0.0F});

    const auto vm = cam.view_matrix();
    const auto im = engine3d::math::matrix4x4{};

    CHECK(vm == im);
}

TEST_CASE("view_matrix returns look_at result")
{
    const camera cam({0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, -1.0F}, {0.0F, 1.0F, 0.0F});

    const auto vm = cam.view_matrix();
    const auto lm = engine3d::math::look_at({0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, -1.0F}, {0.0F, 1.0F, 0.0F});

    CHECK(vm == lm);
}

TEST_CASE("view_matrix identity camera")
{
    const camera cam({0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, -1.0F}, {0.0F, 1.0F, 0.0F});

    const auto vm = cam.view_matrix();
    const auto im = engine3d::math::matrix4x4{};

    CHECK(vm == im);
}

TEST_CASE("view_matrix translates eye position")
{
    const camera cam({1.0F, 2.0F, 3.0F}, {4.0F, 5.0F, 6.0F}, {0.0F, 1.0F, 0.0F});

    const auto vm = cam.view_matrix();
    const engine3d::math::vector4 eye{1.0F, 2.0F, 3.0F, 1.0F};

    const engine3d::math::vector4 result = vm * eye;
    CHECK(result.x() == approx(0.0F));
    CHECK(result.y() == approx(0.0F));
    CHECK(result.z() == approx(0.0F));
}

TEST_CASE("view_matrix orthonormal basis")
{
    const camera cam({1.0F, 2.0F, 3.0F}, {4.0F, 5.0F, 6.0F}, {0.0F, 1.0F, 0.0F});

    const auto vm = cam.view_matrix();
    const engine3d::math::vector3 row0{vm(0, 0), vm(0, 1), vm(0, 2)};
    const engine3d::math::vector3 row1{vm(1, 0), vm(1, 1), vm(1, 2)};
    const engine3d::math::vector3 row2{vm(2, 0), vm(2, 1), vm(2, 2)};

    CHECK(row0.length() == approx(1.0F));
    CHECK(row1.length() == approx(1.0F));
    CHECK(row2.length() == approx(1.0F));

    CHECK(engine3d::math::dot(row0, row1) == approx(0.0F));
    CHECK(engine3d::math::dot(row0, row2) == approx(0.0F));
    CHECK(engine3d::math::dot(row1, row2) == approx(0.0F));
}

TEST_CASE("view_matrix with custom up vector")
{
    const camera cam({0.0F, 0.0F, 0.0F}, {1.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 1.0F});

    const auto vm = cam.view_matrix();
    const engine3d::math::vector3 row0{vm(0, 0), vm(0, 1), vm(0, 2)};
    const engine3d::math::vector3 row1{vm(1, 0), vm(1, 1), vm(1, 2)};
    const engine3d::math::vector3 row2{vm(2, 0), vm(2, 1), vm(2, 2)};

    CHECK(row0.length() == approx(1.0F));
    CHECK(row1.length() == approx(1.0F));
    CHECK(row2.length() == approx(1.0F));

    CHECK(engine3d::math::dot(row0, row1) == approx(0.0F));
    CHECK(engine3d::math::dot(row0, row2) == approx(0.0F));
    CHECK(engine3d::math::dot(row1, row2) == approx(0.0F));
}
