#pragma once

#include <engine3d/math/math.hxx>

namespace engine3d::math {

class vector2 {
public:
    constexpr vector2(float x, float y) noexcept : _x{x}, _y{y} {}

    constexpr float x() const noexcept { return _x; }
    constexpr float y() const noexcept { return _y; }

    vector2& operator+=(const vector2& vc) noexcept;
    vector2& operator-=(const vector2& vc) noexcept;
    vector2& operator*=(float c) noexcept;
    vector2& operator/=(float c);

    float length_squared() const noexcept;
    float length() const noexcept;

    vector2 normalized() const noexcept;
    void normalize() noexcept;

    float dot_product(const vector2& vc) const noexcept;
    float cross_product(const vector2& vc) const noexcept;

private:
    float _x{0.0F};
    float _y{0.0F};
};

vector2 operator+(const vector2& vo, const vector2& vi) noexcept;
vector2 operator-(const vector2& vo, const vector2& vi) noexcept;
vector2 operator-(const vector2& vo) noexcept;
vector2 operator*(vector2 v, float c) noexcept;
vector2 operator/(vector2 v, float c);

bool operator==(const vector2& vo, const vector2& vi) noexcept;
bool operator!=(const vector2& vo, const vector2& vi) noexcept;
bool almost_equal(const vector2& vo, const vector2& vi, float tolerance = epsilon) noexcept;

} // namespace engine3d::math
