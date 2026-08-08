#include "testing.hxx"
#include "vector4.hxx"

using namespace math;

constexpr vector4 test_constexpr_ctor{1.0F, 2.0F, 3.0F, 4.0F};

void vector4_test_add()
{
    const vector4 v0{1.0F, 2.0F, 3.0F, 4.0F};
    const vector4 v1{5.0F, 6.0F, 7.0F, 8.0F};
    const vector4 v2{6.0F, 8.0F, 10.0F, 12.0F};

    EQ(v0 + v1, v2);
}

void vector4_test_length()
{
    const vector4 v0{1.0F, 2.0F, 2.0F, 4.0F};

    FLOAT_EQ(v0.length(), 5.0F);
}

void vector4_test_normalize()
{
    vector4 v0{3.0F, 0.0F, 0.0F, 4.0F};
    v0.normalize();

    FLOAT_EQ(v0.x(), 0.6F);
    FLOAT_EQ(v0.y(), 0.0F);
    FLOAT_EQ(v0.z(), 0.0F);
    FLOAT_EQ(v0.w(), 0.8F);
}

void vector4_test_dot_product()
{
    const vector4 v0{1.0F, 2.0F, 3.0F, 4.0F};
    const vector4 v1{5.0F, 6.0F, 7.0F, 8.0F};

    FLOAT_EQ(v0.dot_product(v1), 70.0F);
}

void vector4_test_equality()
{
    const vector4 a{1.0F, 2.0F, 3.0F, 4.0F};
    const vector4 same{1.0F, 2.0F, 3.0F, 4.0F};
    EQ(a == same, true);
    EQ(a != same, false);

    const vector4 nearby{1.0F + 1e-6F, 2.0F, 3.0F, 4.0F};
    EQ(a == nearby, true);
    EQ(a != nearby, false);

    const vector4 far{1.0F + 1e-3F, 2.0F, 3.0F, 4.0F};
    EQ(a == far, false);
    EQ(a != far, true);
}
