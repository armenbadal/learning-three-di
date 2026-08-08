#pragma once

namespace math {

class vector3 {
public:
    constexpr vector3(float x, float y, float z) : _x{x}, _y{y}, _z{z} {}

    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }

    void operator+=(const vector3& vc);
    void operator-=(const vector3& vc);
    void operator*=(float c);
    void operator/=(float c);

    float length_squared() const;
    float length() const;

    vector3 normalized() const;
    void normalize();

    float dot_product(const vector3& vc) const;
    vector3 cross_product(const vector3& vc) const;

    bool operator==(const vector3&) const = default;

private:
    float _x{0.0F};
    float _y{0.0F};
    float _z{0.0F};
};

vector3 operator+(const vector3& vo, const vector3& vi);
vector3 operator-(const vector3& vo, const vector3& vi);
vector3 operator-(const vector3& vo);
vector3 operator*(vector3 v, float c);
vector3 operator/(vector3 v, float c);

} // namespace math
