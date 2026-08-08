#include "testing.hxx"
#include "vector2.hxx"

using namespace math;

constexpr vector2 test_constexpr_ctor{3.0F, 4.0F};

void vector2_test_add()
{
    vector2 v0{1.0F, 2.0F};
    vector2 v1{3.0F, 4.0F};
    auto v2 = v0 + v1;
    const vector2 v3{4.0F, 6.0F};
    EQ(v2, v3);
}

void vector2_test_length()
{
    vector2 v0{3.0F, 4.0F};
    FLOAT_EQ(v0.length(), 5.0F);
}

void vector2_test_normalize()
{
    vector2 v0{3.0F, 4.0F};
    v0.normalize();
    FLOAT_EQ(v0.x(), 0.6F);
    FLOAT_EQ(v0.y(), 0.8F);
}

void vector2_test_normalize_zero()
{
    vector2 v0{0.0F, 0.0F};
    v0.normalize();
    FLOAT_EQ(v0.x(), 0.0F);
    FLOAT_EQ(v0.y(), 0.0F);

    vector2 v1 = v0.normalized();
    FLOAT_EQ(v1.x(), 0.0F);
    FLOAT_EQ(v1.y(), 0.0F);
}

void vector2_test_equality()
{
    const vector2 a{1.0F, 2.0F};
    const vector2 same{1.0F, 2.0F};
    EQ(a == same, true);
    EQ(a != same, false);

    const vector2 nearby{1.0F + 1e-6F, 2.0F};
    EQ(a == nearby, true);
    EQ(a != nearby, false);

    const vector2 far{1.0F + 1e-3F, 2.0F};
    EQ(a == far, false);
    EQ(a != far, true);
}

/*
int main()
{
    vector2_test_add();
    vector2_test_length();
    vector2_test_normalize();
    vector2_test_normalize_zero();
}
*/
