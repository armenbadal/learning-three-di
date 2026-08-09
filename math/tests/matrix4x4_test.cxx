#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "matrix4x4.hxx"

#include <sstream>
#include <string>
#include <limits>
#include <numbers>

using namespace math;

namespace {
    constexpr float pi = std::numbers::pi_v<float>;

    auto approx = [](float value) { return Catch::Approx(value).margin(1e-5f); };
}

constexpr matrix4x4 test_constexpr_ctor{1.0F, 2.0F, 3.0F, 4.0F,
                                        5.0F, 6.0F, 7.0F, 8.0F,
                                        9.0F, 10.0F, 11.0F, 12.0F,
                                        13.0F, 14.0F, 15.0F, 16.0F};

constexpr matrix4x4 test_constexpr_transpose = test_constexpr_ctor.transpose();
static_assert(test_constexpr_transpose(0, 1) == 5.0F);
static_assert(test_constexpr_transpose(1, 0) == 2.0F);
static_assert(test_constexpr_transpose(3, 0) == 4.0F);
static_assert(test_constexpr_transpose(0, 3) == 13.0F);

TEST_CASE("matrix4x4 constructor")
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};

    CHECK(m(0, 0) == approx(1.0F));
    CHECK(m(0, 3) == approx(4.0F));
    CHECK(m(1, 1) == approx(6.0F));
    CHECK(m(2, 2) == approx(11.0F));
    CHECK(m(3, 2) == approx(15.0F));
    CHECK(m(3, 3) == approx(16.0F));
}

TEST_CASE("matrix4x4 default is identity")
{
    CHECK(matrix4x4{} == matrix4x4::identity());
}

TEST_CASE("matrix4x4 identity")
{
    const matrix4x4 expected{1.0F, 0.0F, 0.0F, 0.0F,
                             0.0F, 1.0F, 0.0F, 0.0F,
                             0.0F, 0.0F, 1.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 1.0F};

    CHECK(matrix4x4::identity() == expected);
}

TEST_CASE("matrix4x4 zero")
{
    const matrix4x4 expected{0.0F, 0.0F, 0.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 0.0F};

    CHECK(matrix4x4::zero() == expected);
}

TEST_CASE("matrix4x4 element access mutable")
{
    matrix4x4 m;
    m(1, 2) = 5.0F;
    CHECK(m(1, 2) == approx(5.0F));

    const matrix4x4& cm = m;
    CHECK(cm(1, 2) == approx(5.0F));
}

TEST_CASE("matrix4x4 multiply identity")
{
    const matrix4x4 a{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};
    const matrix4x4 i;

    CHECK(a * i == a);
    CHECK(i * a == a);
}

TEST_CASE("matrix4x4 multiply scale")
{
    const matrix4x4 a{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};
    const matrix4x4 scale2{2.0F, 0.0F, 0.0F, 0.0F,
                           0.0F, 2.0F, 0.0F, 0.0F,
                           0.0F, 0.0F, 2.0F, 0.0F,
                           0.0F, 0.0F, 0.0F, 2.0F};
    const matrix4x4 expected{2.0F, 4.0F, 6.0F, 8.0F,
                             10.0F, 12.0F, 14.0F, 16.0F,
                             18.0F, 20.0F, 22.0F, 24.0F,
                             26.0F, 28.0F, 30.0F, 32.0F};

    CHECK(a * scale2 == expected);
    CHECK(scale2 * a == expected);
}

TEST_CASE("matrix4x4 multiply vector identity")
{
    const matrix4x4 m;
    const vector4 v{1.0F, 2.0F, 3.0F, 4.0F};

    CHECK(m * v == v);
}

TEST_CASE("matrix4x4 multiply vector scale")
{
    const matrix4x4 m{2.0F, 0.0F, 0.0F, 0.0F,
                      0.0F, 3.0F, 0.0F, 0.0F,
                      0.0F, 0.0F, 4.0F, 0.0F,
                      0.0F, 0.0F, 0.0F, 5.0F};
    const vector4 v{1.0F, 2.0F, 3.0F, 4.0F};

    CHECK(m * v == vector4{2.0F, 6.0F, 12.0F, 20.0F});
}

TEST_CASE("matrix4x4 multiply vector general")
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};
    const vector4 v{1.0F, 1.0F, 1.0F, 1.0F};

    CHECK(m * v == vector4{10.0F, 26.0F, 42.0F, 58.0F});
}

TEST_CASE("matrix4x4 rotation zero is identity")
{
    CHECK(matrix4x4::rotation_x(0.0F) == matrix4x4{});
    CHECK(matrix4x4::rotation_y(0.0F) == matrix4x4{});
    CHECK(matrix4x4::rotation_z(0.0F) == matrix4x4{});
}

TEST_CASE("matrix4x4 rotation z quarter turn")
{
    const matrix4x4 r = matrix4x4::rotation_z(pi / 2.0F);
    const vector4 ex{1.0F, 0.0F, 0.0F, 0.0F};
    const vector4 ey{0.0F, 1.0F, 0.0F, 0.0F};

    const vector4 rx = r * ex;
    CHECK(rx.x() == approx(0.0F));
    CHECK(rx.y() == approx(1.0F));

    const vector4 ry = r * ey;
    CHECK(ry.x() == approx(-1.0F));
    CHECK(ry.y() == approx(0.0F));
}

TEST_CASE("matrix4x4 rotation x quarter turn")
{
    const matrix4x4 r = matrix4x4::rotation_x(pi / 2.0F);
    const vector4 ey{0.0F, 1.0F, 0.0F, 0.0F};
    const vector4 ez{0.0F, 0.0F, 1.0F, 0.0F};

    const vector4 ry = r * ey;
    CHECK(ry.y() == approx(0.0F));
    CHECK(ry.z() == approx(1.0F));

    const vector4 rz = r * ez;
    CHECK(rz.y() == approx(-1.0F));
    CHECK(rz.z() == approx(0.0F));
}

TEST_CASE("matrix4x4 rotation y quarter turn")
{
    const matrix4x4 r = matrix4x4::rotation_y(pi / 2.0F);
    const vector4 ex{1.0F, 0.0F, 0.0F, 0.0F};
    const vector4 ez{0.0F, 0.0F, 1.0F, 0.0F};

    const vector4 rz = r * ez;
    CHECK(rz.x() == approx(1.0F));
    CHECK(rz.z() == approx(0.0F));

    const vector4 rx = r * ex;
    CHECK(rx.x() == approx(0.0F));
    CHECK(rx.z() == approx(-1.0F));
}

TEST_CASE("matrix4x4 rotation inverse")
{
    const float angle = 0.7F;
    CHECK(almost_equal(matrix4x4::rotation_x(angle) * matrix4x4::rotation_x(-angle), matrix4x4{}));
    CHECK(almost_equal(matrix4x4::rotation_y(angle) * matrix4x4::rotation_y(-angle), matrix4x4{}));
    CHECK(almost_equal(matrix4x4::rotation_z(angle) * matrix4x4::rotation_z(-angle), matrix4x4{}));
}

TEST_CASE("matrix4x4 rotation full turn")
{
    const vector4 v{1.0F, 2.0F, 3.0F, 1.0F};

    const vector4 rz = matrix4x4::rotation_z(2.0F * pi) * v;
    CHECK(rz.x() == approx(v.x()));
    CHECK(rz.y() == approx(v.y()));
    CHECK(rz.z() == approx(v.z()));
}

TEST_CASE("matrix4x4 transpose")
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};
    const matrix4x4 t = m.transpose();

    CHECK(t(0, 1) == approx(5.0F));
    CHECK(t(1, 0) == approx(2.0F));
    CHECK(t(3, 0) == approx(4.0F));
    CHECK(t(0, 3) == approx(13.0F));
    CHECK(t(2, 2) == approx(11.0F));
}

TEST_CASE("matrix4x4 transpose inverse")
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};

    CHECK(m.transpose().transpose() == m);
    CHECK(matrix4x4::identity().transpose() == matrix4x4{});
}

TEST_CASE("matrix4x4 determinant basics")
{
    CHECK(matrix4x4{}.determinant() == approx(1.0F));
    CHECK(matrix4x4::zero().determinant() == approx(0.0F));

    const matrix4x4 diag{2.0F, 0.0F, 0.0F, 0.0F,
                         0.0F, 3.0F, 0.0F, 0.0F,
                         0.0F, 0.0F, 4.0F, 0.0F,
                         0.0F, 0.0F, 0.0F, 5.0F};
    CHECK(diag.determinant() == approx(120.0F));
}

TEST_CASE("matrix4x4 determinant transforms")
{
    CHECK(matrix4x4::scaling(vector3{2.0F, 3.0F, 4.0F}).determinant() == approx(24.0F));
    CHECK(matrix4x4::translation(vector3{1.0F, 2.0F, 3.0F}).determinant() == approx(1.0F));
    CHECK(matrix4x4::rotation_z(0.7F).determinant() == approx(1.0F));
    CHECK(matrix4x4::rotation_x(0.7F).determinant() == approx(1.0F));
    CHECK(matrix4x4::rotation_y(0.7F).determinant() == approx(1.0F));
}

TEST_CASE("matrix4x4 determinant singular")
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};

    CHECK(m.determinant() == approx(0.0F));
}

TEST_CASE("matrix4x4 inverse identity")
{
    const auto inv = matrix4x4{}.inverse();
    REQUIRE(inv.has_value());
    CHECK(inv.value() == matrix4x4{});
}

TEST_CASE("matrix4x4 inverse diagonal")
{
    const matrix4x4 m{2.0F, 0.0F, 0.0F, 0.0F,
                      0.0F, 4.0F, 0.0F, 0.0F,
                      0.0F, 0.0F, 0.5F, 0.0F,
                      0.0F, 0.0F, 0.0F, 10.0F};
    const matrix4x4 expected{0.5F, 0.0F, 0.0F, 0.0F,
                             0.0F, 0.25F, 0.0F, 0.0F,
                             0.0F, 0.0F, 2.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 0.1F};

    const auto inv = m.inverse();
    REQUIRE(inv.has_value());
    CHECK(inv.value() == expected);
}

TEST_CASE("matrix4x4 inverse product is identity")
{
    const matrix4x4 m{4.0F, 0.0F, 0.0F, 0.0F,
                      1.0F, 2.0F, 1.0F, 0.0F,
                      0.0F, 0.0F, 3.0F, 1.0F,
                      2.0F, 1.0F, 0.0F, 5.0F};

    const auto inv = m.inverse();
    REQUIRE(inv.has_value());
    CHECK(almost_equal(m * inv.value(), matrix4x4{}));
    CHECK(almost_equal(inv.value() * m, matrix4x4{}));
}

TEST_CASE("matrix4x4 inverse double")
{
    const matrix4x4 m{4.0F, 0.0F, 0.0F, 0.0F,
                      1.0F, 2.0F, 1.0F, 0.0F,
                      0.0F, 0.0F, 3.0F, 1.0F,
                      2.0F, 1.0F, 0.0F, 5.0F};

    const auto inv = m.inverse();
    REQUIRE(inv.has_value());
    CHECK(almost_equal(inv.value().inverse().value(), m));
}

TEST_CASE("matrix4x4 inverse rotation is transpose")
{
    CHECK(almost_equal(matrix4x4::rotation_z(0.7F).inverse().value(), matrix4x4::rotation_z(0.7F).transpose()));
    CHECK(almost_equal(matrix4x4::rotation_x(0.9F).inverse().value(), matrix4x4::rotation_x(0.9F).transpose()));
}

TEST_CASE("matrix4x4 inverse singular")
{
    CHECK_FALSE(matrix4x4::zero().inverse().has_value());
}

TEST_CASE("matrix4x4 inverse affine")
{
    const matrix4x4 m = matrix4x4::translation(vector3{1.0F, 2.0F, 3.0F})
                      * matrix4x4::scaling(vector3{2.0F, 3.0F, 4.0F});
    const auto inv_opt = m.inverse();
    REQUIRE(inv_opt.has_value());
    const matrix4x4 inv = inv_opt.value();

    CHECK(almost_equal(m * inv, matrix4x4{}));
    CHECK(almost_equal(inv * m, matrix4x4{}));

    const vector4 p{3.0F, 4.0F, 5.0F, 1.0F};
    const vector4 q = m * p;
    CHECK(q.x() == approx(7.0F));
    CHECK(q.y() == approx(14.0F));
    CHECK(q.z() == approx(23.0F));

    const vector4 r = inv * q;
    CHECK(r.x() == approx(3.0F));
    CHECK(r.y() == approx(4.0F));
    CHECK(r.z() == approx(5.0F));

    CHECK(inv(3, 0) == approx(0.0F));
    CHECK(inv(3, 1) == approx(0.0F));
    CHECK(inv(3, 2) == approx(0.0F));
    CHECK(inv(3, 3) == approx(1.0F));
}

TEST_CASE("matrix4x4 inverse affine translation")
{
    const matrix4x4 t = matrix4x4::translation(vector3{1.0F, 2.0F, 3.0F});

    CHECK(t.inverse().value() == matrix4x4::translation(vector3{-1.0F, -2.0F, -3.0F}));
}

TEST_CASE("matrix4x4 inverse affine singular")
{
    const matrix4x4 m = matrix4x4::scaling(vector3{2.0F, 0.0F, 4.0F});

    CHECK_FALSE(m.inverse().has_value());
}

TEST_CASE("matrix4x4 inverse non affine")
{
    const matrix4x4 m{4.0F, 0.0F, 0.0F, 0.0F,
                      1.0F, 2.0F, 1.0F, 0.0F,
                      0.0F, 0.0F, 3.0F, 1.0F,
                      2.0F, 1.0F, 0.0F, 5.0F};

    const auto inv = m.inverse();
    REQUIRE(inv.has_value());
    CHECK(almost_equal(m * inv.value(), matrix4x4{}));
    CHECK(almost_equal(inv.value() * m, matrix4x4{}));
}

TEST_CASE("matrix4x4 equality")
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};

    matrix4x4 same = m;
    CHECK(m == same);
    CHECK_FALSE(m != same);

    matrix4x4 nearby = m;
    nearby(0, 0) += 1e-6F;
    CHECK_FALSE(m == nearby);
    CHECK(m != nearby);
    CHECK(almost_equal(m, nearby));

    matrix4x4 far = m;
    far(0, 0) += 1e-3F;
    CHECK_FALSE(m == far);
    CHECK(m != far);
    CHECK_FALSE(almost_equal(m, far));

    CHECK_FALSE(m == matrix4x4::translation(vector3{1.0F, 2.0F, 3.0F}));
    CHECK(m != matrix4x4::translation(vector3{1.0F, 2.0F, 3.0F}));
}

TEST_CASE("matrix4x4 nan is never equal")
{
    matrix4x4 nan = matrix4x4{};
    nan(0, 0) = std::numeric_limits<float>::quiet_NaN();

    CHECK_FALSE(nan == nan);
    CHECK_FALSE(almost_equal(nan, nan));
}

TEST_CASE("matrix4x4 stream output")
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};
    std::ostringstream out;
    out << m;
    const std::string expected =
        " 1.0000  2.0000  3.0000  4.0000\n"
        " 5.0000  6.0000  7.0000  8.0000\n"
        " 9.0000 10.0000 11.0000 12.0000\n"
        "13.0000 14.0000 15.0000 16.0000\n";

    CHECK(out.str() == expected);
}
