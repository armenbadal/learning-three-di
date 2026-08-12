#include "engine3d/math/transform.hxx"

#include <cmath>

namespace e3d::math {

matrix4x4 model_matrix(const vector3& t, const vector3& r, const vector3& s) noexcept
{
    auto tr = translation(t);

    auto rx = rotation_x(r.x());
    auto ry = rotation_y(r.y());
    auto rz = rotation_z(r.z());

    auto sc = scaling(s);

    return tr * (rz * ry * rx) * sc;
}

matrix4x4 look_at(const vector3& e, const vector3& t, const vector3& u)
{
    const auto f = (t - e).normalized();
    const auto r = cross(f, u).normalized();
    const auto s = cross(r, f);

    return matrix4x4{
         r.x(),  r.y(),  r.z(), -dot(r, e),
         s.x(),  s.y(),  s.z(), -dot(s, e),
        -f.x(), -f.y(), -f.z(),  dot(f, e),
         0.0F,   0.0F,   0.0F,  1.0F
    };
}

matrix4x4 translation(const vector3& tr) noexcept
{
    return matrix4x4{
        1.0F, 0.0F, 0.0F, tr.x(),
        0.0F, 1.0F, 0.0F, tr.y(),
        0.0F, 0.0F, 1.0F, tr.z(),
        0.0F, 0.0F, 0.0F, 1.0F
    };
}

matrix4x4 scaling(const vector3& sc) noexcept
{
    return matrix4x4{
        sc.x(),   0.0F,   0.0F, 0.0F,
          0.0F, sc.y(),   0.0F, 0.0F,
          0.0F,   0.0F, sc.z(), 0.0F,
          0.0F,   0.0F,   0.0F, 1.0F
    };
}

matrix4x4 rotation_x(float angle) noexcept
{
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    return matrix4x4{
        1.0F, 0.0F, 0.0F, 0.0F,
        0.0F,    c,   -s, 0.0F,
        0.0F,    s,    c, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    };
}

matrix4x4 rotation_y(float angle) noexcept
{
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    return matrix4x4{
           c, 0.0F,    s, 0.0F,
        0.0F, 1.0F, 0.0F, 0.0F,
          -s, 0.0F,    c, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    };
}

matrix4x4 rotation_z(float angle) noexcept
{
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    return matrix4x4{
           c,   -s, 0.0F, 0.0F,
           s,    c, 0.0F, 0.0F,
        0.0F, 0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    };
}

} // namespace e3d::math