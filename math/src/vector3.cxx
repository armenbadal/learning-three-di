#include "vector3.hxx"

#include <cassert>
#include <cmath>

namespace math {

void vector3::operator+=(const vector3& vc)
{
    _x += vc._x;
    _y += vc._y;
    _z += vc._z;
}

void vector3::operator-=(const vector3& vc)
{
    _x -= vc._x;
    _y -= vc._y;
    _z -= vc._z;
}

void vector3::operator*=(float c)
{
    _x *= c;
    _y *= c;
    _z *= c;
}

void vector3::operator/=(float c)
{
    assert(c != 0.0F);
    _x /= c;
    _y /= c;
    _z /= c;
}

float vector3::length_squared() const
{
    return _x * _x + _y * _y + _z * _z;
}

float vector3::length() const
{
    return std::sqrt(length_squared());
}

vector3 vector3::normalized() const
{
    const float len = length();
    if( len == 0.0F )
        return *this;
    return *this / len;
}

void vector3::normalize()
{
    const float len = length();
    if( len != 0.0F )
        *this /= len;
}

float vector3::dot_product(const vector3& vc) const
{
    return _x * vc._x + _y * vc._y + _z * vc._z;
}

vector3 vector3::cross_product(const vector3& vc) const
{
    auto x = _y * vc._z - _z * vc._y;
    auto y = _x * vc._z - _z * vc._x;
    auto z = _x * vc._y - _y * vc._x;
    return vector3{x, -y, z};
}

vector3 operator+(const vector3& vo, const vector3& vi)
{
    return vector3{vo.x() + vi.x(), vo.y() + vi.y(), vo.z() + vi.z()};
}

vector3 operator-(const vector3& vo, const vector3& vi)
{
    return vector3{vo.x() - vi.x(), vo.y() - vi.y(), vo.z() - vi.z()};
}

vector3 operator-(const vector3& vo)
{
    return vector3{-vo.x(), -vo.y(), -vo.z()};
}

vector3 operator*(vector3 v, float c)
{
    v *= c;
    return v;
}

vector3 operator/(vector3 v, float c)
{
    v /= c;
    return v;
}

bool operator==(const vector3& vo, const vector3& vi)
{
    return vo.x() == vi.x() && vo.y() == vi.y() && vo.z() == vi.z();
}

bool operator!=(const vector3& vo, const vector3& vi)
{
    return !(vo == vi);
}

bool almost_equal(const vector3& vo, const vector3& vi, float tolerance)
{
    return almost_equal(vo.x(), vi.x(), tolerance)
        && almost_equal(vo.y(), vi.y(), tolerance)
        && almost_equal(vo.z(), vi.z(), tolerance);
}

} // namespace math
