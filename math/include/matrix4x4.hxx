#pragma once

#include "vector4.hxx"

namespace math {

class matrix4x4 {
public:
    matrix4x4() = default;
    
    matrix4x4(float e00, float e01, float e02, float e03,
              float e10, float e11, float e12, float e13,
              float e20, float e21, float e22, float e23,
              float e30, float e31, float e32, float e33);

    static matrix4x4 identity();
    static matrix4x4 zero();

    float operator()(unsigned int row, unsigned int column) const;
    float& operator()(unsigned int row, unsigned int column);

    vector4 row(unsigned int row) const;
    vector4 column(unsigned int column) const;

    bool operator==(const matrix4x4&) const = default;

private:
    float _m[4][4] = {{1.0F, 0.0F, 0.0F, 0.0F},
                      {0.0F, 1.0F, 0.0F, 0.0F},
                      {0.0F, 0.0F, 1.0F, 0.0F},
                      {0.0F, 0.0F, 0.0F, 1.0F}};
};

matrix4x4 operator*(const matrix4x4& u, const matrix4x4& v);
vector4 operator*(const matrix4x4& m, const vector4& v);

} // namespace math
