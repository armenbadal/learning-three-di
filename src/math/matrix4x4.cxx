module e3d.math;

import std;

namespace e3d::math {

float matrix4x4::determinant3x3(
            float a, float b, float c,
            float d, float e, float f,
            float g, float h, float i) noexcept
{
    return a * (e * i - f * h)
         - b * (d * i - f * g)
         + c * (d * h - e * g);
}

float matrix4x4::determinant() const noexcept
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

std::optional<matrix4x4> matrix4x4::inverse() const noexcept
{
    if( _m[3][0] == 0.0F && _m[3][1] == 0.0F && _m[3][2] == 0.0F && _m[3][3] == 1.0F )
        return inverse_affine();
    return inverse_general();
}

std::optional<matrix4x4> matrix4x4::inverse_affine() const noexcept
{
    const float l00 = _m[0][0], l01 = _m[0][1], l02 = _m[0][2];
    const float l10 = _m[1][0], l11 = _m[1][1], l12 = _m[1][2];
    const float l20 = _m[2][0], l21 = _m[2][1], l22 = _m[2][2];
    const float tx = _m[0][3], ty = _m[1][3], tz = _m[2][3];

    const float det = determinant3x3(l00, l01, l02, l10, l11, l12, l20, l21, l22);
    if( std::fabs(det) < epsilon )
        return std::nullopt;

    const float c00 = l11 * l22 - l12 * l21;
    const float c01 = l12 * l20 - l10 * l22;
    const float c02 = l10 * l21 - l11 * l20;
    const float c10 = l21 * l02 - l22 * l01;
    const float c11 = l22 * l00 - l20 * l02;
    const float c12 = l20 * l01 - l21 * l00;
    const float c20 = l01 * l12 - l02 * l11;
    const float c21 = l02 * l10 - l00 * l12;
    const float c22 = l00 * l11 - l01 * l10;

    const float linv00 = c00 / det, linv01 = c10 / det, linv02 = c20 / det;
    const float linv10 = c01 / det, linv11 = c11 / det, linv12 = c21 / det;
    const float linv20 = c02 / det, linv21 = c12 / det, linv22 = c22 / det;

    const float itx = -(linv00 * tx + linv01 * ty + linv02 * tz);
    const float ity = -(linv10 * tx + linv11 * ty + linv12 * tz);
    const float itz = -(linv20 * tx + linv21 * ty + linv22 * tz);

    return matrix4x4{
        linv00, linv01, linv02, itx,
        linv10, linv11, linv12, ity,
        linv20, linv21, linv22, itz,
        0.0F, 0.0F, 0.0F, 1.0F
    };
}

std::optional<matrix4x4> matrix4x4::inverse_general() const noexcept
{
    const float det = determinant();
    if( std::fabs(det) < epsilon )
        return std::nullopt;

    matrix4x4 result = matrix4x4::zero();
    for( unsigned int r = 0; r < 4; ++r )
        for( unsigned int c = 0; c < 4; ++c ) {
            float minor[3][3];
            unsigned int mi = 0;
            for( unsigned int i = 0; i < 4; ++i ) {
                if( i == r )
                    continue;
                unsigned int mj = 0;
                for( unsigned int j = 0; j < 4; ++j ) {
                    if( j == c )
                        continue;
                    minor[mi][mj] = _m[i][j];
                    ++mj;
                }
                ++mi;
            }
            const float cofactor = ((r + c) % 2 == 0 ? 1.0F : -1.0F)
                * determinant3x3(minor[0][0], minor[0][1], minor[0][2],
                                 minor[1][0], minor[1][1], minor[1][2],
                                 minor[2][0], minor[2][1], minor[2][2]);
            result(c, r) = cofactor / det;
        }

    return result;
}

matrix4x4 matrix4x4::identity() noexcept
{
    return matrix4x4{};
}

matrix4x4 matrix4x4::zero() noexcept
{
    return matrix4x4{
        0.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 0.0F
    };
}

float& matrix4x4::operator()(unsigned int row, unsigned int column) noexcept
{
    return _m[row][column];
}

vector4 matrix4x4::row(unsigned int row) const noexcept
{
    return vector4{_m[row][0], _m[row][1], _m[row][2], _m[row][3]};
}

vector4 matrix4x4::column(unsigned int column) const noexcept
{
    return vector4{_m[0][column], _m[1][column], _m[2][column], _m[3][column]};
}

matrix4x4 operator*(const matrix4x4& u, const matrix4x4& v) noexcept
{
    matrix4x4 result = matrix4x4::zero();
    for( unsigned int r = 0; r < 4; ++r ) {
        const auto row = u.row(r);
        for( unsigned int c = 0; c < 4; ++c )
            result(r, c) = dot(row, v.column(c));
    }
    return result;
}

vector4 operator*(const matrix4x4& m, const vector4& v) noexcept
{
    return vector4{
        dot(m.row(0), v),
        dot(m.row(1), v),
        dot(m.row(2), v),
        dot(m.row(3), v)
    };
}

std::ostream& operator<<(std::ostream& out, const matrix4x4& m)
{
    for( unsigned int r = 0; r < 4; ++r )
        out << std::format("{:7.4f} {:7.4f} {:7.4f} {:7.4f}\n", 
                           m(r, 0), m(r, 1), m(r, 2), m(r, 3));
    return out;
}

bool operator==(const matrix4x4& mo, const matrix4x4& mi) noexcept
{
    for( unsigned int r = 0; r < 4; ++r )
        for( unsigned int c = 0; c < 4; ++c )
            if( mo(r,c) != mi(r,c) )
                return false;
    return true;
}

bool operator!=(const matrix4x4& mo, const matrix4x4& mi) noexcept
{
    return !(mo == mi);
}

bool almost_equal(const matrix4x4& mo, const matrix4x4& mi, float tolerance) noexcept
{
    for( unsigned int r = 0; r < 4; ++r )
        for( unsigned int c = 0; c < 4; ++c )
            if( !almost_equal(mo(r,c), mi(r,c), tolerance) )
                return false;
    return true;
}

} // namespace e3d::math
