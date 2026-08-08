#include "matrix4x4.hxx"

#include <cmath>

namespace math {

float matrix4x4::determinant3x3(
            float a, float b, float c,
            float d, float e, float f,
            float g, float h, float i) const
{
    return a * (e * i - f * h)
         - b * (d * i - f * g)
         + c * (d * h - e * g);
}

float matrix4x4::determinant() const
{
    const float minor00 = determinant3x3(
        _m[1][1], _m[1][2], _m[1][3],
        _m[2][1], _m[2][2], _m[2][3],
        _m[3][1], _m[3][2], _m[3][3]);
    const float minor01 = determinant3x3(
        _m[1][0], _m[1][2], _m[1][3],
        _m[2][0], _m[2][2], _m[2][3],
        _m[3][0], _m[3][2], _m[3][3]);
    const float minor02 = determinant3x3(
        _m[1][0], _m[1][1], _m[1][3],
        _m[2][0], _m[2][1], _m[2][3],
        _m[3][0], _m[3][1], _m[3][3]);
    const float minor03 = determinant3x3(
        _m[1][0], _m[1][1], _m[1][2],
        _m[2][0], _m[2][1], _m[2][2],
        _m[3][0], _m[3][1], _m[3][2]);

    return _m[0][0] * minor00
         - _m[0][1] * minor01
         + _m[0][2] * minor02
         - _m[0][3] * minor03;
}

matrix4x4 matrix4x4::identity()
{
    return matrix4x4{};
}

matrix4x4 matrix4x4::zero()
{
    return matrix4x4{
        0.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 0.0F
    };
}

matrix4x4 matrix4x4::translation(vector3 tr)
{
    return matrix4x4{
        1.0F, 0.0F, 0.0F, tr.x(),
        0.0F, 1.0F, 0.0F, tr.y(),
        0.0F, 0.0F, 1.0F, tr.z(),
        0.0F, 0.0F, 0.0F, 1.0F
    };
}

matrix4x4 matrix4x4::scaling(vector3 sc)
{
    return matrix4x4{
        sc.x(),   0.0F,   0.0F, 0.0F,
          0.0F, sc.y(),   0.0F, 0.0F,
          0.0F,   0.0F, sc.z(), 0.0F,
          0.0F,   0.0F,   0.0F, 1.0F
    };
}

matrix4x4 matrix4x4::rotation_x(float angle)
{
    const float c = std::cosf(angle);
    const float s = std::sinf(angle);
    return matrix4x4{
        1.0F, 0.0F, 0.0F, 0.0F,
        0.0F,    c,   -s, 0.0F,
        0.0F,    s,    c, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    };
}

matrix4x4 matrix4x4::rotation_y(float angle)
{
    const float c = std::cosf(angle);
    const float s = std::sinf(angle);
    return matrix4x4{
           c, 0.0F,    s, 0.0F,
        0.0F, 1.0F, 0.0F, 0.0F,
          -s, 0.0F,    c, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    };
}

matrix4x4 matrix4x4::rotation_z(float angle)
{
    const float c = std::cosf(angle);
    const float s = std::sinf(angle);
    return matrix4x4{
           c,   -s, 0.0F, 0.0F,
           s,    c, 0.0F, 0.0F,
        0.0F, 0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    };
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

