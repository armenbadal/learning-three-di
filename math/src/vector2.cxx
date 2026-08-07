#include "vector2.hxx"

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
    return *this / length();
}

void vector2::normalize()
{
    *this /= length();
}

float vector2::dot_product(const vector2& vc)
{
    return _x * vc._x + _y * vc._y;
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

} // namespace math

