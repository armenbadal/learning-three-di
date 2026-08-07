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

vector4 matrix4x4::row(unsigned int row) const
{
    return vector4{_m[row][0], _m[row][1], _m[row][2], _m[row][3]};
}

vector4 matrix4x4::column(unsigned int column) const
{
    return vector4{_m[0][column], _m[1][column], _m[2][column], _m[3][column]};
}

matrix4x4 operator*(const matrix4x4& u, const matrix4x4& v)
{
    matrix4x4 result = matrix4x4::zero();
    for( unsigned int r = 0; r < 4; ++r ) {
        const auto row = u.row(r);
        for( unsigned int c = 0; c < 4; ++c )
            result(r, c) = row.dot_product(v.column(c));
    }
    return result;
}

vector4 operator*(const matrix4x4& m, const vector4& v)
{
    return vector4{
        m.row(0).dot_product(v),
        m.row(1).dot_product(v),
        m.row(2).dot_product(v),
        m.row(3).dot_product(v)
    };
}

} // namespace math

