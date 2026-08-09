#include "vector2.hxx"

#include <cassert>
#include <cmath>

namespace math {

void vector2::operator+=(const vector2& vc)
{
    _x += vc._x;
    _y += vc._y;
}

void vector2::operator-=(const vector2& vc)
{
    _x -= vc._x;
    _y -= vc._y;
}

void vector2::operator*=(float c)
{
    _x *= c;
    _y *= c;
}

void vector2::operator/=(float c)
{
    assert(c != 0.0F);
    _x /= c;
    _y /= c;
}

float vector2::length_squared() const
{
    return _x * _x + _y * _y;
}

float vector2::length() const
{
    return std::sqrt(length_squared());
}

vector2 vector2::normalized() const
{
    const float len = length();
    if( len == 0.0F )
        return *this;
    return *this / len;
}

void vector2::normalize()
{
    const float len = length();
    if( len != 0.0F )
        *this /= len;
}

float vector2::dot_product(const vector2& vc) const
{
    return _x * vc._x + _y * vc._y;
}

float vector2::cross_product(const vector2& vc) const
{
    return _x * vc._y - _y * vc._x;
}

vector2 operator+(const vector2& vo, const vector2& vi)
{
    return vector2{vo.x() + vi.x(), vo.y() + vi.y()};
}

vector2 operator-(const vector2& vo, const vector2& vi)
{
    return vector2{vo.x() - vi.x(), vo.y() - vi.y()};
}

vector2 operator-(const vector2& vo)
{
    return vector2{-vo.x(), -vo.y()};
}

vector2 operator*(vector2 v, float c)
{
    v *= c;
    return v;
}

vector2 operator/(vector2 v, float c)
{
    v /= c;
    return v;
}

bool operator==(const vector2& vo, const vector2& vi)
{
    return vo.x() == vi.x() && vo.y() == vi.y();
}

bool operator!=(const vector2& vo, const vector2& vi)
{
    return !(vo == vi);
}

bool almost_equal(const vector2& vo, const vector2& vi, float tolerance)
{
    return almost_equal(vo.x(), vi.x(), tolerance)
        && almost_equal(vo.y(), vi.y(), tolerance);
}

} // namespace math
