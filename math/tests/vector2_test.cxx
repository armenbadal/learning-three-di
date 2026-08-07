#include "testing.hxx"
#include "vector2.hxx"

using namespace math;

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

/*
int main()
{
    vector2_test_add();
    vector2_test_length();
    vector2_test_normalize();
    vector2_test_normalize_zero();
}
*/
