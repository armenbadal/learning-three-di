#include "testing.hxx"

void vector2_test_add();
void vector2_test_length();
void vector2_test_normalize();
void vector2_test_normalize_zero();

void vector3_test_cross_product_basis();
void vector3_test_cross_product_anticommutative();
void vector3_test_cross_product_parallel_is_zero();
void vector3_test_cross_product_perpendicular();

void vector4_test_add();
void vector4_test_length();
void vector4_test_normalize();
void vector4_test_dot_product();

int main()
{
    vector2_test_add();
    vector2_test_length();
    vector2_test_normalize();
    vector2_test_normalize_zero();

    vector3_test_cross_product_basis();
    vector3_test_cross_product_anticommutative();
    vector3_test_cross_product_parallel_is_zero();
    vector3_test_cross_product_perpendicular();

    vector4_test_add();
    vector4_test_length();
    vector4_test_normalize();
    vector4_test_dot_product();

    return test::report();
}
