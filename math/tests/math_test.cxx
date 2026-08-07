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

void matrix4x4_test_constructor();
void matrix4x4_test_default_is_identity();
void matrix4x4_test_identity();
void matrix4x4_test_zero();
void matrix4x4_test_element_access_mutable();
void matrix4x4_test_multiply_identity();
void matrix4x4_test_multiply_scale();
void matrix4x4_test_multiply_vector_identity();
void matrix4x4_test_multiply_vector_scale();
void matrix4x4_test_multiply_vector_general();
void matrix4x4_test_rotation_zero_is_identity();
void matrix4x4_test_rotation_z_quarter_turn();
void matrix4x4_test_rotation_x_quarter_turn();
void matrix4x4_test_rotation_y_quarter_turn();
void matrix4x4_test_rotation_inverse();
void matrix4x4_test_rotation_full_turn();

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

    matrix4x4_test_constructor();
    matrix4x4_test_default_is_identity();
    matrix4x4_test_identity();
    matrix4x4_test_zero();
    matrix4x4_test_element_access_mutable();
    matrix4x4_test_multiply_identity();
    matrix4x4_test_multiply_scale();
    matrix4x4_test_multiply_vector_identity();
    matrix4x4_test_multiply_vector_scale();
    matrix4x4_test_multiply_vector_general();
    matrix4x4_test_rotation_zero_is_identity();
    matrix4x4_test_rotation_z_quarter_turn();
    matrix4x4_test_rotation_x_quarter_turn();
    matrix4x4_test_rotation_y_quarter_turn();
    matrix4x4_test_rotation_inverse();
    matrix4x4_test_rotation_full_turn();

    return test::report();
}
