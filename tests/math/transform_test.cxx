#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <numbers>

import e3d;

using namespace e3d::math;

namespace {
    constexpr float pi = std::numbers::pi_v<float>;

    auto approx = [](float value) { return Catch::Approx(value).margin(1e-5f); };
}

TEST_CASE("model_matrix identity")
{
    const auto m = model_matrix({0.0F, 0.0F, 0.0F},
                                {0.0F, 0.0F, 0.0F},
                                {1.0F, 1.0F, 1.0F});

    CHECK(m == matrix4x4{});
}

TEST_CASE("model_matrix translation")
{
    const auto m = model_matrix({1.0F, 2.0F, 3.0F},
                                {0.0F, 0.0F, 0.0F},
                                {1.0F, 1.0F, 1.0F});
    const vector4 p{0.0F, 0.0F, 0.0F, 1.0F};

    const vector4 result = m * p;
    CHECK(result.x() == approx(1.0F));
    CHECK(result.y() == approx(2.0F));
    CHECK(result.z() == approx(3.0F));
}

TEST_CASE("model_matrix scaling")
{
    const auto m = model_matrix({0.0F, 0.0F, 0.0F},
                                {0.0F, 0.0F, 0.0F},
                                {2.0F, 3.0F, 4.0F});
    const vector4 p{1.0F, 1.0F, 1.0F, 1.0F};

    const vector4 result = m * p;
    CHECK(result.x() == approx(2.0F));
    CHECK(result.y() == approx(3.0F));
    CHECK(result.z() == approx(4.0F));
}

TEST_CASE("model_matrix rotation")
{
    const auto m = model_matrix({0.0F, 0.0F, 0.0F},
                                {0.0F, 0.0F, pi / 2.0F},
                                {1.0F, 1.0F, 1.0F});
    const vector4 p{1.0F, 0.0F, 0.0F, 1.0F};

    const vector4 result = m * p;
    CHECK(result.x() == approx(0.0F));
    CHECK(result.y() == approx(1.0F));
    CHECK(result.z() == approx(0.0F));
}

TEST_CASE("model_matrix trs order")
{
    const auto m = model_matrix({1.0F, 0.0F, 0.0F},
                                {0.0F, 0.0F, 0.0F},
                                {2.0F, 2.0F, 2.0F});
    const vector4 p{1.0F, 1.0F, 1.0F, 1.0F};

    const vector4 result = m * p;
    CHECK(result.x() == approx(3.0F));
    CHECK(result.y() == approx(2.0F));
    CHECK(result.z() == approx(2.0F));

    const vector4 direction{1.0F, 1.0F, 1.0F, 0.0F};
    const vector4 dr = m * direction;
    CHECK(dr.x() == approx(2.0F));
    CHECK(dr.y() == approx(2.0F));
    CHECK(dr.z() == approx(2.0F));
}

TEST_CASE("translation")
{
    const auto m = translation({1.0F, 2.0F, 3.0F});
    const vector4 p{5.0F, 6.0F, 7.0F, 1.0F};

    const vector4 result = m * p;
    CHECK(result.x() == approx(6.0F));
    CHECK(result.y() == approx(8.0F));
    CHECK(result.z() == approx(10.0F));
}

TEST_CASE("scaling")
{
    const auto m = scaling({2.0F, 3.0F, 4.0F});
    const vector4 p{1.0F, 1.0F, 1.0F, 1.0F};

    const vector4 result = m * p;
    CHECK(result.x() == approx(2.0F));
    CHECK(result.y() == approx(3.0F));
    CHECK(result.z() == approx(4.0F));
}

TEST_CASE("rotation_x")
{
    const auto m = rotation_x(pi / 2.0F);
    const vector4 ey{0.0F, 1.0F, 0.0F, 0.0F};
    const vector4 ez{0.0F, 0.0F, 1.0F, 0.0F};

    CHECK((m * ey).y() == approx(0.0F));
    CHECK((m * ey).z() == approx(1.0F));
    CHECK((m * ez).y() == approx(-1.0F));
    CHECK((m * ez).z() == approx(0.0F));
}

TEST_CASE("rotation_y")
{
    const auto m = rotation_y(pi / 2.0F);
    const vector4 ex{1.0F, 0.0F, 0.0F, 0.0F};
    const vector4 ez{0.0F, 0.0F, 1.0F, 0.0F};

    CHECK((m * ez).x() == approx(1.0F));
    CHECK((m * ez).z() == approx(0.0F));
    CHECK((m * ex).x() == approx(0.0F));
    CHECK((m * ex).z() == approx(-1.0F));
}

TEST_CASE("rotation_z")
{
    const auto m = rotation_z(pi / 2.0F);
    const vector4 ex{1.0F, 0.0F, 0.0F, 1.0F};
    const vector4 ey{0.0F, 1.0F, 0.0F, 1.0F};

    CHECK((m * ex).x() == approx(0.0F));
    CHECK((m * ex).y() == approx(1.0F));
    CHECK((m * ey).x() == approx(-1.0F));
    CHECK((m * ey).y() == approx(0.0F));
}

TEST_CASE("look_at standard orientation")
{
    const auto m = look_at({0.0F, 0.0F, 0.0F},
                           {0.0F, 0.0F, -1.0F},
                           {0.0F, 1.0F, 0.0F});

    const vector4 right{1.0F, 0.0F, 0.0F, 0.0F};
    const vector4 up{0.0F, 1.0F, 0.0F, 0.0F};
    const vector4 forward{0.0F, 0.0F, -1.0F, 0.0F};

    CHECK((m * right).x() == approx(1.0F));
    CHECK((m * right).y() == approx(0.0F));
    CHECK((m * right).z() == approx(0.0F));

    CHECK((m * up).x() == approx(0.0F));
    CHECK((m * up).y() == approx(1.0F));
    CHECK((m * up).z() == approx(0.0F));

    CHECK((m * forward).x() == approx(0.0F));
    CHECK((m * forward).y() == approx(0.0F));
    CHECK((m * forward).z() == approx(-1.0F));
}

TEST_CASE("look_at translation")
{
    const auto m = look_at({1.0F, 2.0F, 3.0F},
                           {4.0F, 2.0F, 3.0F},
                           {0.0F, 1.0F, 0.0F});

    const vector4 eye{1.0F, 2.0F, 3.0F, 1.0F};
    const vector4 result = m * eye;

    CHECK(result.x() == approx(0.0F));
    CHECK(result.y() == approx(0.0F));
    CHECK(result.z() == approx(0.0F));
}

TEST_CASE("look_at arbitrary up vector")
{
    const auto m = look_at({0.0F, 0.0F, 0.0F},
                           {1.0F, 0.0F, 0.0F},
                           {0.0F, 0.0F, 1.0F});

    const vector4 right{1.0F, 0.0F, 0.0F, 0.0F};
    const vector4 up{0.0F, 0.0F, 1.0F, 0.0F};
    const vector4 forward{0.0F, -1.0F, 0.0F, 0.0F};

    CHECK((m * right).x() == approx(0.0F));
    CHECK((m * right).y() == approx(0.0F));
    CHECK((m * right).z() == approx(-1.0F));

    CHECK((m * up).x() == approx(0.0F));
    CHECK((m * up).y() == approx(1.0F));
    CHECK((m * up).z() == approx(0.0F));

    CHECK((m * forward).x() == approx(1.0F));
    CHECK((m * forward).y() == approx(0.0F));
    CHECK((m * forward).z() == approx(0.0F));
}

TEST_CASE("look_at orthonormal basis")
{
    const auto m = look_at({1.0F, 2.0F, 3.0F},
                           {4.0F, 5.0F, 6.0F},
                           {0.0F, 1.0F, 0.0F});

    const auto r0 = m.row(0);
    const auto r1 = m.row(1);
    const auto r2 = m.row(2);

    CHECK(std::sqrt(r0.x() * r0.x() + r0.y() * r0.y() + r0.z() * r0.z()) == approx(1.0F));
    CHECK(std::sqrt(r1.x() * r1.x() + r1.y() * r1.y() + r1.z() * r1.z()) == approx(1.0F));
    CHECK(std::sqrt(r2.x() * r2.x() + r2.y() * r2.y() + r2.z() * r2.z()) == approx(1.0F));

    CHECK(dot({r0.x(), r0.y(), r0.z()}, {r1.x(), r1.y(), r1.z()}) == approx(0.0F));
    CHECK(dot({r0.x(), r0.y(), r0.z()}, {r2.x(), r2.y(), r2.z()}) == approx(0.0F));
    CHECK(dot({r1.x(), r1.y(), r1.z()}, {r2.x(), r2.y(), r2.z()}) == approx(0.0F));
}

TEST_CASE("look_at diagonal view direction")
{
    const vector3 eye{0.0F, 0.0F, 0.0F};
    const vector3 target{1.0F, 1.0F, 1.0F};
    const vector3 up{0.0F, 1.0F, 0.0F};

    const auto m = look_at(eye, target, up);

    const vector4 result = m * vector4{1.0F, 1.0F, 1.0F, 1.0F};
    CHECK(result.x() == approx(0.0F));
    CHECK(result.y() == approx(0.0F));
    CHECK(result.z() == approx(-std::numbers::sqrt3_v<float>));
}

TEST_CASE("perspective maps near and far planes to normalized device coordinates")
{
    constexpr float near_plane = 1.0F;
    constexpr float far_plane = 10.0F;
    const auto projection = perspective(pi / 2.0F, 1.0F, near_plane, far_plane);

    const auto near_clip = projection * vector4{0.0F, 0.0F, -near_plane, 1.0F};
    const auto far_clip = projection * vector4{0.0F, 0.0F, -far_plane, 1.0F};

    CHECK(near_clip.z() / near_clip.w() == approx(-1.0F));
    CHECK(far_clip.z() / far_clip.w() == approx(1.0F));
}

TEST_CASE("perspective applies vertical field of view and aspect ratio")
{
    const auto projection = perspective(pi / 2.0F, 2.0F, 1.0F, 10.0F);

    CHECK(projection(0, 0) == approx(0.5F));
    CHECK(projection(1, 1) == approx(1.0F));
    CHECK(projection(3, 2) == approx(-1.0F));
}
