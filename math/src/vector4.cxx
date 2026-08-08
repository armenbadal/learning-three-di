#include "vector4.hxx"

#include <cmath>

namespace math {

void vector4::operator+=(const vector4& vc)
{
    _x += vc._x;
    _y += vc._y;
    _z += vc._z;
    _w += vc._w;
}

void vector4::operator-=(const vector4& vc)
{
    _x -= vc._x;
    _y -= vc._y;
    _z -= vc._z;
    _w -= vc._w;
}

void vector4::operator*=(float c)
{
    _x *= c;
    _y *= c;
    _z *= c;
    _w *= c;
}

void vector4::operator/=(float c)
{
    if( c == 0.0F ) return;
    _x /= c;
    _y /= c;
    _z /= c;
    _w /= c;
}

float vector4::length_squared() const
{
    return _x * _x + _y * _y + _z * _z + _w * _w;
}

float vector4::length() const
{
    return std::sqrt(length_squared());
}

vector4 vector4::normalized() const
{
    const float len = length();
    if( len == 0.0F )
        return *this;
    return *this / len;
}

void vector4::normalize()
{
    const float len = length();
    if( len != 0.0F )
        *this /= len;
}

float vector4::dot_product(const vector4& vc) const
{
    return _x * vc._x + _y * vc._y + _z * vc._z + _w * vc._w;
}

vector4 operator+(const vector4& vo, const vector4& vi)
{
    return vector4{vo.x() + vi.x(), vo.y() + vi.y(), vo.z() + vi.z(), vo.w() + vi.w()};
}

vector4 operator-(const vector4& vo, const vector4& vi)
{
    return vector4{vo.x() - vi.x(), vo.y() - vi.y(), vo.z() - vi.z(), vo.w() - vi.w()};
}

vector4 operator-(const vector4& vo)
{
    return vector4{-vo.x(), -vo.y(), -vo.z(), -vo.w()};
}

vector4 operator*(vector4 v, float c)
{
    v *= c;
    return v;
}

vector4 operator/(vector4 v, float c)
{
    v /= c;
    return v;
}

bool operator==(const vector4& vo, const vector4& vi)
{
    constexpr float epsilon = 1e-5f;
    return fabs(vo.x() - vi.x()) <= epsilon
        && fabs(vo.y() - vi.y()) <= epsilon
        && fabs(vo.z() - vi.z()) <= epsilon
        && fabs(vo.w() - vi.w()) <= epsilon;
}

bool operator!=(const vector4& vo, const vector4& vi)
{
    return !(vo == vi);
}

} // namespace math