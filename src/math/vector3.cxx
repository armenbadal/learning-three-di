#include "engine3d/math/vector3.hxx"

#include <cmath>
#include <stdexcept>

namespace engine3d::math {

vector3& vector3::operator+=(const vector3& vc) noexcept
{
    _x += vc._x;
    _y += vc._y;
    _z += vc._z;
    return *this;
}

vector3& vector3::operator-=(const vector3& vc) noexcept
{
    _x -= vc._x;
    _y -= vc._y;
    _z -= vc._z;
    return *this;
}

vector3& vector3::operator*=(float c) noexcept
{
    _x *= c;
    _y *= c;
    _z *= c;
    return *this;
}

vector3& vector3::operator/=(float c)
{
    if( c == 0.0F )
        throw std::domain_error("division by zero");
    _x /= c;
    _y /= c;
    _z /= c;
    return *this;
}

float vector3::length_squared() const noexcept
{
    return _x * _x + _y * _y + _z * _z;
}

float vector3::length() const noexcept
{
    return std::sqrt(length_squared());
}

vector3 vector3::normalized() const noexcept
{
    const float len = length();
    if( len == 0.0F )
        return *this;
    return *this / len;
}

void vector3::normalize() noexcept
{
    const float len = length();
    if( len != 0.0F )
        *this /= len;
}

vector3 operator+(const vector3& vo, const vector3& vi) noexcept
{
    return vector3{vo.x() + vi.x(), vo.y() + vi.y(), vo.z() + vi.z()};
}

vector3 operator-(const vector3& vo, const vector3& vi) noexcept
{
    return vector3{vo.x() - vi.x(), vo.y() - vi.y(), vo.z() - vi.z()};
}

vector3 operator-(const vector3& vo) noexcept
{
    return vector3{-vo.x(), -vo.y(), -vo.z()};
}

vector3 operator*(vector3 v, float c) noexcept
{
    v *= c;
    return v;
}

vector3 operator/(vector3 v, float c)
{
    v /= c;
    return v;
}

float dot(const vector3& vo, const vector3& vi) noexcept
{
    return vo.x() * vi.x() + vo.y() * vi.y() + vo.z() * vi.z();
}

vector3 cross(const vector3& vo, const vector3& vi) noexcept
{
    const auto x = vo.y() * vi.z() - vo.z() * vi.y();
    const auto y = vo.x() * vi.z() - vo.z() * vi.x();
    const auto z = vo.x() * vi.y() - vo.y() * vi.x();
    return vector3{x, -y, z};
}

bool operator==(const vector3& vo, const vector3& vi) noexcept
{
    return vo.x() == vi.x() && vo.y() == vi.y() && vo.z() == vi.z();
}

bool operator!=(const vector3& vo, const vector3& vi) noexcept
{
    return !(vo == vi);
}

bool almost_equal(const vector3& vo, const vector3& vi, float tolerance) noexcept
{
    return almost_equal(vo.x(), vi.x(), tolerance)
        && almost_equal(vo.y(), vi.y(), tolerance)
        && almost_equal(vo.z(), vi.z(), tolerance);
}

} // namespace engine3d::math
