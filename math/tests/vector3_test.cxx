#include "testing.hxx"
#include "vector3.hxx"

using namespace math;

constexpr vector3 test_constexpr_ctor{1.0F, 2.0F, 3.0F};

void vector3_test_cross_product_basis()
{
    const vector3 x{1.0F, 0.0F, 0.0F};
    const vector3 y{0.0F, 1.0F, 0.0F};
    const vector3 z{0.0F, 0.0F, 1.0F};

    EQ(x.cross_product(y), z);
    EQ(y.cross_product(z), x);
    EQ(z.cross_product(x), y);
}

void vector3_test_cross_product_anticommutative()
{
    const vector3 a{1.0F, 2.0F, 3.0F};
    const vector3 b{4.0F, 5.0F, 6.0F};

    EQ(a.cross_product(b), -b.cross_product(a));
}

void vector3_test_cross_product_parallel_is_zero()
{
    const vector3 a{2.0F, 4.0F, 6.0F};
    const vector3 b{1.0F, 2.0F, 3.0F};
    const vector3 zero{0.0F, 0.0F, 0.0F};

    EQ(a.cross_product(b), zero);
}

void vector3_test_cross_product_perpendicular()
{
    const vector3 a{1.0F, 2.0F, 3.0F};
    const vector3 b{4.0F, 5.0F, 6.0F};
    const vector3 c = a.cross_product(b);

    FLOAT_EQ(c.dot_product(a), 0.0F);
    FLOAT_EQ(c.dot_product(b), 0.0F);
}

void vector3_test_equality()
{
    const vector3 a{1.0F, 2.0F, 3.0F};
    const vector3 same{1.0F, 2.0F, 3.0F};
    EQ(a == same, true);
    EQ(a != same, false);

    const vector3 nearby{1.0F + 1e-6F, 2.0F, 3.0F};
    EQ(a == nearby, true);
    EQ(a != nearby, false);

    const vector3 far{1.0F + 1e-3F, 2.0F, 3.0F};
    EQ(a == far, false);
    EQ(a != far, true);
}
