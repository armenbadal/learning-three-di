#pragma once

#include "vector3.hxx"
#include "vector4.hxx"

namespace math {

class matrix4x4 {
public:
    matrix4x4() = default;
    
    constexpr matrix4x4(float e00, float e01, float e02, float e03,
                        float e10, float e11, float e12, float e13,
                        float e20, float e21, float e22, float e23,
                        float e30, float e31, float e32, float e33)
        : _m{{e00, e01, e02, e03},
             {e10, e11, e12, e13},
             {e20, e21, e22, e23},
             {e30, e31, e32, e33}}
    {}

    constexpr matrix4x4 transpose() const
    {
        matrix4x4 result{};
        for (unsigned int r = 0; r < 4; ++r)
            for (unsigned int c = 0; c < 4; ++c)
                result._m[c][r] = _m[r][c];
        return result;
    }

    float determinant() const;
    matrix4x4 inverse() const;

    static matrix4x4 identity();
    static matrix4x4 zero();
    static matrix4x4 translation(vector3 translation);
    static matrix4x4 scaling(vector3 scaling);
    static matrix4x4 rotation_x(float angle);
    static matrix4x4 rotation_y(float angle);
    static matrix4x4 rotation_z(float angle);

    constexpr float operator()(unsigned int row, unsigned int column) const { return _m[row][column]; }
    float& operator()(unsigned int row, unsigned int column);

    vector4 row(unsigned int row) const;
    vector4 column(unsigned int column) const;

    bool operator==(const matrix4x4&) const = default;

private:
    float determinant3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i) const;

    matrix4x4 inverse_affine() const;
    matrix4x4 inverse_general() const;

    float _m[4][4] = {{1.0F, 0.0F, 0.0F, 0.0F},
                      {0.0F, 1.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, 1.0F, 0.0F},
                      {0.0F, 0.0F, 0.0F, 1.0F}};
};

matrix4x4 operator*(const matrix4x4& u, const matrix4x4& v);
vector4 operator*(const matrix4x4& m, const vector4& v);

} // namespace math
