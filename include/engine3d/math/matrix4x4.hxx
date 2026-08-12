#pragma once

#include "engine3d/math/math.hxx"
#include "engine3d/math/vector4.hxx"

#include <array>
#include <optional>
#include <ostream>

namespace e3d::math {

class matrix4x4 {
    using row_type = std::array<float, 4>;

public:
    constexpr matrix4x4() noexcept = default;
    
    constexpr matrix4x4(float e00, float e01, float e02, float e03,
                        float e10, float e11, float e12, float e13,
                        float e20, float e21, float e22, float e23,
                        float e30, float e31, float e32, float e33) noexcept
        : _m{{row_type{e00, e01, e02, e03},
              row_type{e10, e11, e12, e13},
              row_type{e20, e21, e22, e23},
              row_type{e30, e31, e32, e33}}}
    {}

    constexpr matrix4x4 transpose() const noexcept
    {
        matrix4x4 result{};
        for (unsigned int r = 0; r < 4; ++r)
            for (unsigned int c = 0; c < 4; ++c)
                result._m[c][r] = _m[r][c];
        return result;
    }

    float determinant() const noexcept;
    std::optional<matrix4x4> inverse() const noexcept;

    static matrix4x4 identity() noexcept;
    static matrix4x4 zero() noexcept;

    constexpr float operator()(unsigned int row, unsigned int column) const noexcept { return _m[row][column]; }
    float& operator()(unsigned int row, unsigned int column) noexcept;

    vector4 row(unsigned int row) const noexcept;
    vector4 column(unsigned int column) const noexcept;

private:
    static float determinant3x3(
        float a, float b, float c, float d, float e, float f, float g, float h, float i) noexcept;

    std::optional<matrix4x4> inverse_affine() const noexcept;
    std::optional<matrix4x4> inverse_general() const noexcept;

    // Row-major storage. Matrices multiply column vectors on the left.
    std::array<row_type, 4> _m{{row_type{1.0F, 0.0F, 0.0F, 0.0F},
                                row_type{0.0F, 1.0F, 0.0F, 0.0F},
                                row_type{0.0F, 0.0F, 1.0F, 0.0F},
                                row_type{0.0F, 0.0F, 0.0F, 1.0F}}};
};

matrix4x4 operator*(const matrix4x4& u, const matrix4x4& v) noexcept;
vector4 operator*(const matrix4x4& m, const vector4& v) noexcept;

std::ostream& operator<<(std::ostream& out, const matrix4x4& m);

bool operator==(const matrix4x4& mo, const matrix4x4& mi) noexcept;
bool operator!=(const matrix4x4& mo, const matrix4x4& mi) noexcept;
bool almost_equal(const matrix4x4& mo, const matrix4x4& mi, float tolerance = epsilon) noexcept;

} // namespace e3d::math
