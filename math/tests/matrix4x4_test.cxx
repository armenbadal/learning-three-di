#include "testing.hxx"
#include "matrix4x4.hxx"

using namespace math;

namespace {
    constexpr float pi = 3.14159265358979323846F;

    void matrix_close(const matrix4x4& a, const matrix4x4& b)
    {
        for (unsigned int r = 0; r < 4; ++r)
            for (unsigned int c = 0; c < 4; ++c)
                FLOAT_EQ(a(r, c), b(r, c));
    }
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

void matrix4x4_test_constructor()
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};

    FLOAT_EQ(m(0, 0), 1.0F);
    FLOAT_EQ(m(0, 3), 4.0F);
    FLOAT_EQ(m(1, 1), 6.0F);
    FLOAT_EQ(m(2, 2), 11.0F);
    FLOAT_EQ(m(3, 2), 15.0F);
    FLOAT_EQ(m(3, 3), 16.0F);
}

void matrix4x4_test_default_is_identity()
{
    matrix4x4 m;
    EQ(m, matrix4x4::identity());
}

void matrix4x4_test_identity()
{
    const matrix4x4 value = matrix4x4::identity();
    const matrix4x4 expected{1.0F, 0.0F, 0.0F, 0.0F,
                             0.0F, 1.0F, 0.0F, 0.0F,
                             0.0F, 0.0F, 1.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 1.0F};
    EQ(value, expected);
}

void matrix4x4_test_zero()
{
    matrix4x4 m = matrix4x4::zero();
    const matrix4x4 expected{0.0F, 0.0F, 0.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 0.0F};
    EQ(m, expected);
}

void matrix4x4_test_element_access_mutable()
{
    matrix4x4 m;
    m(1, 2) = 5.0F;
    FLOAT_EQ(m(1, 2), 5.0F);

    const matrix4x4& cm = m;
    FLOAT_EQ(cm(1, 2), 5.0F);
}

void matrix4x4_test_multiply_identity()
{
    const matrix4x4 a{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};

    const matrix4x4 i;

    EQ(a * i, a);
    EQ(i * a, a);
}

void matrix4x4_test_multiply_scale()
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

    EQ(a * scale2, expected);
    EQ(scale2 * a, expected);
}

void matrix4x4_test_multiply_vector_identity()
{
    const matrix4x4 m;
    const vector4 v{1.0F, 2.0F, 3.0F, 4.0F};

    EQ(m * v, v);
}

void matrix4x4_test_multiply_vector_scale()
{
    const matrix4x4 m{2.0F, 0.0F, 0.0F, 0.0F,
                      0.0F, 3.0F, 0.0F, 0.0F,
                      0.0F, 0.0F, 4.0F, 0.0F,
                      0.0F, 0.0F, 0.0F, 5.0F};
    const vector4 v{1.0F, 2.0F, 3.0F, 4.0F};
    const vector4 expected{2.0F, 6.0F, 12.0F, 20.0F};

    EQ(m * v, expected);
}

void matrix4x4_test_multiply_vector_general()
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};
    const vector4 v{1.0F, 1.0F, 1.0F, 1.0F};
    const vector4 expected{10.0F, 26.0F, 42.0F, 58.0F};

    EQ(m * v, expected);
}

void matrix4x4_test_rotation_zero_is_identity()
{
    EQ(matrix4x4::rotation_x(0.0F), matrix4x4{});
    EQ(matrix4x4::rotation_y(0.0F), matrix4x4{});
    EQ(matrix4x4::rotation_z(0.0F), matrix4x4{});
}

void matrix4x4_test_rotation_z_quarter_turn()
{
    const matrix4x4 r = matrix4x4::rotation_z(pi / 2.0F);
    const vector4 ex{1.0F, 0.0F, 0.0F, 0.0F};
    const vector4 ey{0.0F, 1.0F, 0.0F, 0.0F};

    const vector4 rx = r * ex;
    FLOAT_EQ(rx.x(), 0.0F);
    FLOAT_EQ(rx.y(), 1.0F);

    const vector4 ry = r * ey;
    FLOAT_EQ(ry.x(), -1.0F);
    FLOAT_EQ(ry.y(), 0.0F);
}

void matrix4x4_test_rotation_x_quarter_turn()
{
    const matrix4x4 r = matrix4x4::rotation_x(pi / 2.0F);
    const vector4 ey{0.0F, 1.0F, 0.0F, 0.0F};
    const vector4 ez{0.0F, 0.0F, 1.0F, 0.0F};

    const vector4 ry = r * ey;
    FLOAT_EQ(ry.y(), 0.0F);
    FLOAT_EQ(ry.z(), 1.0F);

    const vector4 rz = r * ez;
    FLOAT_EQ(rz.y(), -1.0F);
    FLOAT_EQ(rz.z(), 0.0F);
}

void matrix4x4_test_rotation_y_quarter_turn()
{
    const matrix4x4 r = matrix4x4::rotation_y(pi / 2.0F);
    const vector4 ex{1.0F, 0.0F, 0.0F, 0.0F};
    const vector4 ez{0.0F, 0.0F, 1.0F, 0.0F};

    const vector4 rz = r * ez;
    FLOAT_EQ(rz.x(), 1.0F);
    FLOAT_EQ(rz.z(), 0.0F);

    const vector4 rx = r * ex;
    FLOAT_EQ(rx.x(), 0.0F);
    FLOAT_EQ(rx.z(), -1.0F);
}

void matrix4x4_test_rotation_inverse()
{
    const float angle = 0.7F;
    matrix_close(matrix4x4::rotation_x(angle) * matrix4x4::rotation_x(-angle), matrix4x4{});
    matrix_close(matrix4x4::rotation_y(angle) * matrix4x4::rotation_y(-angle), matrix4x4{});
    matrix_close(matrix4x4::rotation_z(angle) * matrix4x4::rotation_z(-angle), matrix4x4{});
}

void matrix4x4_test_rotation_full_turn()
{
    const vector4 v{1.0F, 2.0F, 3.0F, 1.0F};

    const vector4 rz = matrix4x4::rotation_z(2.0F * pi) * v;
    FLOAT_EQ(rz.x(), v.x());
    FLOAT_EQ(rz.y(), v.y());
    FLOAT_EQ(rz.z(), v.z());
}

void matrix4x4_test_transpose()
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};
    const matrix4x4 t = m.transpose();

    FLOAT_EQ(t(0, 1), 5.0F);
    FLOAT_EQ(t(1, 0), 2.0F);
    FLOAT_EQ(t(3, 0), 4.0F);
    FLOAT_EQ(t(0, 3), 13.0F);
    FLOAT_EQ(t(2, 2), 11.0F);
}

void matrix4x4_test_transpose_inverse()
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};

    matrix_close(m.transpose().transpose(), m);
    matrix_close(matrix4x4::identity().transpose(), matrix4x4{});
}

void matrix4x4_test_determinant_basics()
{
    FLOAT_EQ(matrix4x4{}.determinant(), 1.0F);
    FLOAT_EQ(matrix4x4::zero().determinant(), 0.0F);

    const matrix4x4 diag{2.0F, 0.0F, 0.0F, 0.0F,
                         0.0F, 3.0F, 0.0F, 0.0F,
                         0.0F, 0.0F, 4.0F, 0.0F,
                         0.0F, 0.0F, 0.0F, 5.0F};
    FLOAT_EQ(diag.determinant(), 120.0F);
}

void matrix4x4_test_determinant_transforms()
{
    FLOAT_EQ(matrix4x4::scaling(vector3{2.0F, 3.0F, 4.0F}).determinant(), 24.0F);
    FLOAT_EQ(matrix4x4::translation(vector3{1.0F, 2.0F, 3.0F}).determinant(), 1.0F);
    FLOAT_EQ(matrix4x4::rotation_z(0.7F).determinant(), 1.0F);
    FLOAT_EQ(matrix4x4::rotation_x(0.7F).determinant(), 1.0F);
    FLOAT_EQ(matrix4x4::rotation_y(0.7F).determinant(), 1.0F);
}

void matrix4x4_test_determinant_singular()
{
    const matrix4x4 m{1.0F, 2.0F, 3.0F, 4.0F,
                      5.0F, 6.0F, 7.0F, 8.0F,
                      9.0F, 10.0F, 11.0F, 12.0F,
                      13.0F, 14.0F, 15.0F, 16.0F};

    FLOAT_EQ(m.determinant(), 0.0F);
}

void matrix4x4_test_inverse_identity()
{
    matrix_close(matrix4x4{}.inverse(), matrix4x4{});
}

void matrix4x4_test_inverse_diagonal()
{
    const matrix4x4 m{2.0F, 0.0F, 0.0F, 0.0F,
                      0.0F, 4.0F, 0.0F, 0.0F,
                      0.0F, 0.0F, 0.5F, 0.0F,
                      0.0F, 0.0F, 0.0F, 10.0F};
    const matrix4x4 expected{0.5F, 0.0F, 0.0F, 0.0F,
                             0.0F, 0.25F, 0.0F, 0.0F,
                             0.0F, 0.0F, 2.0F, 0.0F,
                             0.0F, 0.0F, 0.0F, 0.1F};

    matrix_close(m.inverse(), expected);
}

void matrix4x4_test_inverse_product_is_identity()
{
    const matrix4x4 m{4.0F, 0.0F, 0.0F, 0.0F,
                      1.0F, 2.0F, 1.0F, 0.0F,
                      0.0F, 0.0F, 3.0F, 1.0F,
                      2.0F, 1.0F, 0.0F, 5.0F};

    matrix_close(m * m.inverse(), matrix4x4{});
    matrix_close(m.inverse() * m, matrix4x4{});
}

void matrix4x4_test_inverse_double()
{
    const matrix4x4 m{4.0F, 0.0F, 0.0F, 0.0F,
                           1.0F, 2.0F, 1.0F, 0.0F,
                           0.0F, 0.0F, 3.0F, 1.0F,
                           2.0F, 1.0F, 0.0F, 5.0F};

    matrix_close(m.inverse().inverse(), m);
}

void matrix4x4_test_inverse_rotation_is_transpose()
{
    matrix_close(matrix4x4::rotation_z(0.7F).inverse(), matrix4x4::rotation_z(0.7F).transpose());
    matrix_close(matrix4x4::rotation_x(0.9F).inverse(), matrix4x4::rotation_x(0.9F).transpose());
}

void matrix4x4_test_inverse_singular()
{
    EQ(matrix4x4::zero().inverse(), matrix4x4::zero());
}

void matrix4x4_test_inverse_affine()
{
    const matrix4x4 m = matrix4x4::translation(vector3{1.0F, 2.0F, 3.0F})
                      * matrix4x4::scaling(vector3{2.0F, 3.0F, 4.0F});
    const matrix4x4 inv = m.inverse();

    matrix_close(m * inv, matrix4x4{});
    matrix_close(inv * m, matrix4x4{});

    const vector4 p{3.0F, 4.0F, 5.0F, 1.0F};
    const vector4 q = m * p;
    FLOAT_EQ(q.x(), 7.0F);
    FLOAT_EQ(q.y(), 14.0F);
    FLOAT_EQ(q.z(), 23.0F);

    const vector4 r = inv * q;
    FLOAT_EQ(r.x(), 3.0F);
    FLOAT_EQ(r.y(), 4.0F);
    FLOAT_EQ(r.z(), 5.0F);

    FLOAT_EQ(inv(3, 0), 0.0F);
    FLOAT_EQ(inv(3, 1), 0.0F);
    FLOAT_EQ(inv(3, 2), 0.0F);
    FLOAT_EQ(inv(3, 3), 1.0F);
}

void matrix4x4_test_inverse_affine_translation()
{
    const matrix4x4 t = matrix4x4::translation(vector3{1.0F, 2.0F, 3.0F});
    const matrix4x4 inv = t.inverse();

    matrix_close(inv, matrix4x4::translation(vector3{-1.0F, -2.0F, -3.0F}));
}

void matrix4x4_test_inverse_non_affine()
{
    const matrix4x4 m{4.0F, 0.0F, 0.0F, 0.0F,
                      1.0F, 2.0F, 1.0F, 0.0F,
                      0.0F, 0.0F, 3.0F, 1.0F,
                      2.0F, 1.0F, 0.0F, 5.0F};

    matrix_close(m * m.inverse(), matrix4x4{});
    matrix_close(m.inverse() * m, matrix4x4{});
}