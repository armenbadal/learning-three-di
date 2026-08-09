#pragma once

#include "math.hxx"

namespace math {

class vector2 {
public:
    constexpr vector2(float x, float y) : _x{x}, _y{y} {}

    constexpr float x() const { return _x; }
    constexpr float y() const { return _y; }

    void operator+=(const vector2& vc);
    void operator-=(const vector2& vc);
    void operator*=(float c);
    void operator/=(float c);

    float length_squared() const;
    float length() const;

    vector2 normalized() const;
    void normalize();

    float dot_product(const vector2& vc) const;
    float cross_product(const vector2& vc) const;

private:
    float _x{0.0F};
    float _y{0.0F};
};

vector2 operator+(const vector2& vo, const vector2& vi);
vector2 operator-(const vector2& vo, const vector2& vi);
vector2 operator-(const vector2& vo);
vector2 operator*(vector2 v, float c);
vector2 operator/(vector2 v, float c);

bool operator==(const vector2& vo, const vector2& vi);
bool operator!=(const vector2& vo, const vector2& vi);
bool almost_equal(const vector2& vo, const vector2& vi, float tolerance = epsilon);

} // namespace math
