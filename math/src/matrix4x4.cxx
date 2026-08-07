#include "matrix4x4.hxx"

namespace math {

matrix4x4::matrix4x4(
            float e00, float e01, float e02, float e03,
            float e10, float e11, float e12, float e13,
            float e20, float e21, float e22, float e23,
            float e30, float e31, float e32, float e33)
    : _m{{e00, e01, e02, e03},
         {e10, e11, e12, e13},
         {e20, e21, e22, e23},
         {e30, e31, e32, e33}}
{}

matrix4x4 matrix4x4::identity()
{
    return matrix4x4{};
}

matrix4x4 matrix4x4::zero()
{
    return matrix4x4{0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
}

float matrix4x4::operator()(unsigned int row, unsigned int column) const
{
    return _m[row][column];
}

float& matrix4x4::operator()(unsigned int row, unsigned int column)
{
    return _m[row][column];
}

} // namespace math

