#include "testing.hxx"
#include "matrix4x4.hxx"

using namespace math;

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