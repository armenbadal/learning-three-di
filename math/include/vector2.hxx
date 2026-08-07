#pragma once

namespace math {

class vector2 {
public:
    vector2(float x, float y) : _x{x}, _y{y} {}

    float x() const { return _x; }
    float y() const { return _y; }

    void operator+=(const vector2& vc);
    void operator-=(const vector2& vc);
    void operator*=(float c);
    void operator/=(float c);

    float length_squared() const;
    float length() const;

    vector2 normalized() const;
    void normalize();

    float dot_product(const vector2& vc);

    bool operator==(const vector2&) const = default;

private:
    float _x{0.0F};
    float _y{0.0F};
};

vector2 operator+(const vector2& vo, const vector2& vi);
vector2 operator-(const vector2& vo, const vector2& vi);
vector2 operator-(const vector2& vo);
vector2 operator*(vector2 v, float c);
vector2 operator/(vector2 v, float c);

} // namespace math
