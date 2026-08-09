#pragma once

#include "math.hxx"

namespace math {

class vector4 {
public:
    constexpr vector4(float x, float y, float z, float w) : _x{x}, _y{y}, _z{z}, _w{w} {}

    constexpr float x() const { return _x; }
    constexpr float y() const { return _y; }
    constexpr float z() const { return _z; }
    constexpr float w() const { return _w; }

    void operator+=(const vector4& vc);
    void operator-=(const vector4& vc);
    void operator*=(float c);
    void operator/=(float c);

    float length_squared() const;
    float length() const;

    vector4 normalized() const;
    void normalize();

    float dot_product(const vector4& vc) const;

private:
    float _x{0.0F};
    float _y{0.0F};
    float _z{0.0F};
    float _w{0.0F};
};

vector4 operator+(const vector4& vo, const vector4& vi);
vector4 operator-(const vector4& vo, const vector4& vi);
vector4 operator-(const vector4& vo);
vector4 operator*(vector4 v, float c);
vector4 operator/(vector4 v, float c);

bool operator==(const vector4& vo, const vector4& vi);
bool operator!=(const vector4& vo, const vector4& vi);
bool almost_equal(const vector4& vo, const vector4& vi, float tolerance = epsilon);

} // namespace math
