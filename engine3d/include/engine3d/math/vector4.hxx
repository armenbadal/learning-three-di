#pragma once

#include "engine3d/math/math.hxx"

namespace engine3d::math {

class vector4 {
public:
    constexpr vector4(float x, float y, float z, float w) noexcept : _x{x}, _y{y}, _z{z}, _w{w} {}

    constexpr float x() const noexcept { return _x; }
    constexpr float y() const noexcept { return _y; }
    constexpr float z() const noexcept { return _z; }
    constexpr float w() const noexcept { return _w; }

    vector4& operator+=(const vector4& vc) noexcept;
    vector4& operator-=(const vector4& vc) noexcept;
    vector4& operator*=(float c) noexcept;
    vector4& operator/=(float c);

    float length_squared() const noexcept;
    float length() const noexcept;

    vector4 normalized() const noexcept;
    void normalize() noexcept;

    float dot_product(const vector4& vc) const noexcept;

private:
    float _x{0.0F};
    float _y{0.0F};
    float _z{0.0F};
    float _w{0.0F};
};

vector4 operator+(const vector4& vo, const vector4& vi) noexcept;
vector4 operator-(const vector4& vo, const vector4& vi) noexcept;
vector4 operator-(const vector4& vo) noexcept;
vector4 operator*(vector4 v, float c) noexcept;
vector4 operator/(vector4 v, float c);

bool operator==(const vector4& vo, const vector4& vi) noexcept;
bool operator!=(const vector4& vo, const vector4& vi) noexcept;
bool almost_equal(const vector4& vo, const vector4& vi, float tolerance = epsilon) noexcept;

} // namespace engine3d::math
