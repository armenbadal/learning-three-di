#pragma once

#include "engine3d/math/math.hxx"

namespace e3d::math {

class vector3 {
public:
    constexpr vector3(float x, float y, float z) noexcept : _x{x}, _y{y}, _z{z} {}

    constexpr float x() const noexcept { return _x; }
    constexpr float y() const noexcept { return _y; }
    constexpr float z() const noexcept { return _z; }

    vector3& operator+=(const vector3& vc) noexcept;
    vector3& operator-=(const vector3& vc) noexcept;
    vector3& operator*=(float c) noexcept;
    vector3& operator/=(float c);

    float length_squared() const noexcept;
    float length() const noexcept;

    vector3 normalized() const noexcept;
    void normalize() noexcept;

private:
    float _x{0.0F};
    float _y{0.0F};
    float _z{0.0F};
};

vector3 operator+(const vector3& vo, const vector3& vi) noexcept;
vector3 operator-(const vector3& vo, const vector3& vi) noexcept;
vector3 operator-(const vector3& vo) noexcept;
vector3 operator*(vector3 v, float c) noexcept;
vector3 operator/(vector3 v, float c);

float dot(const vector3& vo, const vector3& vi) noexcept;
vector3 cross(const vector3& vo, const vector3& vi) noexcept;

bool operator==(const vector3& vo, const vector3& vi) noexcept;
bool operator!=(const vector3& vo, const vector3& vi) noexcept;
bool almost_equal(const vector3& vo, const vector3& vi, float tolerance = epsilon) noexcept;

} // namespace e3d::math
