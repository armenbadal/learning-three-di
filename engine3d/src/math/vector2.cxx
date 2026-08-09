#include <engine3d/math/vector2.hxx>

#include <cmath>
#include <stdexcept>

namespace engine3d::math {

vector2& vector2::operator+=(const vector2& vc) noexcept
{
    _x += vc._x;
    _y += vc._y;
    return *this;
}

vector2& vector2::operator-=(const vector2& vc) noexcept
{
    _x -= vc._x;
    _y -= vc._y;
    return *this;
}

vector2& vector2::operator*=(float c) noexcept
{
    _x *= c;
    _y *= c;
    return *this;
}

vector2& vector2::operator/=(float c)
{
    if( c == 0.0F )
        throw std::domain_error("division by zero");
    _x /= c;
    _y /= c;
    return *this;
}

float vector2::length_squared() const noexcept
{
    return _x * _x + _y * _y;
}

float vector2::length() const noexcept
{
    return std::sqrt(length_squared());
}

vector2 vector2::normalized() const noexcept
{
    const float len = length();
    if( len == 0.0F )
        return *this;
    return *this / len;
}

void vector2::normalize() noexcept
{
    const float len = length();
    if( len != 0.0F )
        *this /= len;
}

float vector2::dot_product(const vector2& vc) const noexcept
{
    return _x * vc._x + _y * vc._y;
}

float vector2::cross_product(const vector2& vc) const noexcept
{
    return _x * vc._y - _y * vc._x;
}

vector2 operator+(const vector2& vo, const vector2& vi) noexcept
{
    return vector2{vo.x() + vi.x(), vo.y() + vi.y()};
}

vector2 operator-(const vector2& vo, const vector2& vi) noexcept
{
    return vector2{vo.x() - vi.x(), vo.y() - vi.y()};
}

vector2 operator-(const vector2& vo) noexcept
{
    return vector2{-vo.x(), -vo.y()};
}

vector2 operator*(vector2 v, float c) noexcept
{
    v *= c;
    return v;
}

vector2 operator/(vector2 v, float c)
{
    v /= c;
    return v;
}

bool operator==(const vector2& vo, const vector2& vi) noexcept
{
    return vo.x() == vi.x() && vo.y() == vi.y();
}

bool operator!=(const vector2& vo, const vector2& vi) noexcept
{
    return !(vo == vi);
}

bool almost_equal(const vector2& vo, const vector2& vi, float tolerance) noexcept
{
    return almost_equal(vo.x(), vi.x(), tolerance)
        && almost_equal(vo.y(), vi.y(), tolerance);
}

} // namespace engine3d::math
