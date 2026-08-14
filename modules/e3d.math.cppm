export module e3d.math;

import std;

export namespace e3d::math {

inline constexpr float epsilon = 1e-5f;

inline bool almost_equal(float a, float b, float tolerance = epsilon) noexcept
{
    if( a == b )
        return true;
    if( !std::isfinite(a) || !std::isfinite(b) )
        return false;

    const float scale = std::max(1.0F, std::max(std::fabs(a), std::fabs(b)));
    return std::fabs(a - b) <= tolerance * scale;
}

class vector2 {
public:
    constexpr vector2(float x, float y) noexcept : _x{x}, _y{y} {}

    constexpr float x() const noexcept { return _x; }
    constexpr float y() const noexcept { return _y; }

    vector2& operator+=(const vector2& vc) noexcept;
    vector2& operator-=(const vector2& vc) noexcept;
    vector2& operator*=(float c) noexcept;
    vector2& operator/=(float c);

    float length_squared() const noexcept;
    float length() const noexcept;

    vector2 normalized() const noexcept;
    void normalize() noexcept;

private:
    float _x{0.0F};
    float _y{0.0F};
};

vector2 operator+(const vector2& vo, const vector2& vi) noexcept;
vector2 operator-(const vector2& vo, const vector2& vi) noexcept;
vector2 operator-(const vector2& vo) noexcept;
vector2 operator*(vector2 v, float c) noexcept;
vector2 operator/(vector2 v, float c);

float dot(const vector2& vo, const vector2& vi) noexcept;
float cross(const vector2& vo, const vector2& vi) noexcept;

bool operator==(const vector2& vo, const vector2& vi) noexcept;
bool operator!=(const vector2& vo, const vector2& vi) noexcept;
bool almost_equal(const vector2& vo, const vector2& vi, float tolerance = epsilon) noexcept;

class vector3 {
public:
    constexpr vector3(float x, float y, float z) noexcept : _x{x}, _y{y}, _z{z} {}

    constexpr float x() const noexcept { return _x; }
    constexpr float y() const noexcept { return _y; }
    constexpr float z() const noexcept { return _z; }

    vector3& operator+=(const vector3& vc) noexcept;
    vector3& operator-=(const vector3& vc) noexcept;
    vector3& operator*=(float c) noexcept;
    vector3& operator/=(float c);

    float length_squared() const noexcept;
    float length() const noexcept;

    vector3 normalized() const noexcept;
    void normalize() noexcept;

private:
    float _x{0.0F};
    float _y{0.0F};
    float _z{0.0F};
};

vector3 operator+(const vector3& vo, const vector3& vi) noexcept;
vector3 operator-(const vector3& vo, const vector3& vi) noexcept;
vector3 operator-(const vector3& vo) noexcept;
vector3 operator*(vector3 v, float c) noexcept;
vector3 operator/(vector3 v, float c);

float dot(const vector3& vo, const vector3& vi) noexcept;
vector3 cross(const vector3& vo, const vector3& vi) noexcept;

bool operator==(const vector3& vo, const vector3& vi) noexcept;
bool operator!=(const vector3& vo, const vector3& vi) noexcept;
bool almost_equal(const vector3& vo, const vector3& vi, float tolerance = epsilon) noexcept;

class vector4 {
public:
    constexpr vector4(float x, float y, float z, float w) noexcept : _x{x}, _y{y}, _z{z}, _w{w} {}
    constexpr vector4(const vector3& v, float w) noexcept : vector4{v.x(), v.y(), v.z(), w} {}

    constexpr float x() const noexcept { return _x; }
    constexpr float y() const noexcept { return _y; }
    constexpr float z() const noexcept { return _z; }
    constexpr float w() const noexcept { return _w; }

    vector4& operator+=(const vector4& vc) noexcept;
    vector4& operator-=(const vector4& vc) noexcept;
    vector4& operator*=(float c) noexcept;
    vector4& operator/=(float c);

    float length_squared() const noexcept;
    float length() const noexcept;

    vector4 normalized() const noexcept;
    void normalize() noexcept;

private:
    float _x{0.0F};
    float _y{0.0F};
    float _z{0.0F};
    float _w{0.0F};
};

vector4 operator+(const vector4& vo, const vector4& vi) noexcept;
vector4 operator-(const vector4& vo, const vector4& vi) noexcept;
vector4 operator-(const vector4& vo) noexcept;
vector4 operator*(vector4 v, float c) noexcept;
vector4 operator/(vector4 v, float c);

float dot(const vector4& vo, const vector4& vi) noexcept;

bool operator==(const vector4& vo, const vector4& vi) noexcept;
bool operator!=(const vector4& vo, const vector4& vi) noexcept;
bool almost_equal(const vector4& vo, const vector4& vi, float tolerance = epsilon) noexcept;

class matrix4x4 {
    using row_type = std::array<float, 4>;

public:
    constexpr matrix4x4() noexcept = default;

    constexpr matrix4x4(float e00, float e01, float e02, float e03,
                        float e10, float e11, float e12, float e13,
                        float e20, float e21, float e22, float e23,
                        float e30, float e31, float e32, float e33) noexcept
        : _m{{row_type{e00, e01, e02, e03},
              row_type{e10, e11, e12, e13},
              row_type{e20, e21, e22, e23},
              row_type{e30, e31, e32, e33}}}
    {}

    constexpr matrix4x4 transpose() const noexcept
    {
        matrix4x4 result{};
        for( unsigned int r = 0; r < 4; ++r )
            for( unsigned int c = 0; c < 4; ++c )
                result._m[c][r] = _m[r][c];
        return result;
    }

    float determinant() const noexcept;
    std::optional<matrix4x4> inverse() const noexcept;

    static matrix4x4 identity() noexcept;
    static matrix4x4 zero() noexcept;

    constexpr float operator()(unsigned int row, unsigned int column) const noexcept { return _m[row][column]; }
    float& operator()(unsigned int row, unsigned int column) noexcept;

    vector4 row(unsigned int row) const noexcept;
    vector4 column(unsigned int column) const noexcept;

private:
    static float determinant3x3(
        float a, float b, float c, float d, float e, float f, float g, float h, float i) noexcept;

    std::optional<matrix4x4> inverse_affine() const noexcept;
    std::optional<matrix4x4> inverse_general() const noexcept;

    // Row-major storage. Matrices multiply column vectors on the left.
    std::array<row_type, 4> _m{{row_type{1.0F, 0.0F, 0.0F, 0.0F},
                                row_type{0.0F, 1.0F, 0.0F, 0.0F},
                                row_type{0.0F, 0.0F, 1.0F, 0.0F},
                                row_type{0.0F, 0.0F, 0.0F, 1.0F}}};
};

matrix4x4 operator*(const matrix4x4& u, const matrix4x4& v) noexcept;
vector4 operator*(const matrix4x4& m, const vector4& v) noexcept;

std::ostream& operator<<(std::ostream& out, const matrix4x4& m);

bool operator==(const matrix4x4& mo, const matrix4x4& mi) noexcept;
bool operator!=(const matrix4x4& mo, const matrix4x4& mi) noexcept;
bool almost_equal(const matrix4x4& mo, const matrix4x4& mi, float tolerance = epsilon) noexcept;

matrix4x4 model_matrix(const vector3& t, const vector3& r, const vector3& s) noexcept;
matrix4x4 look_at(const vector3& e, const vector3& t, const vector3& u);
matrix4x4 translation(const vector3& tr) noexcept;
matrix4x4 scaling(const vector3& sc) noexcept;
matrix4x4 rotation_x(float angle) noexcept;
matrix4x4 rotation_y(float angle) noexcept;
matrix4x4 rotation_z(float angle) noexcept;
matrix4x4 perspective(float fov_y, float aspect, float near_plane, float far_plane);

} // namespace e3d::math