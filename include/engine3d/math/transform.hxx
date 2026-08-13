#pragma once

#include "engine3d/math/matrix4x4.hxx"
#include "engine3d/math/vector3.hxx"

namespace e3d::math {

matrix4x4 model_matrix(const vector3& t, const vector3& r, const vector3& s) noexcept;
matrix4x4 look_at(const vector3& e, const vector3& t, const vector3& u);
matrix4x4 translation(const vector3& tr) noexcept;
matrix4x4 scaling(const vector3& sc) noexcept;
matrix4x4 rotation_x(float angle) noexcept;
matrix4x4 rotation_y(float angle) noexcept;
matrix4x4 rotation_z(float angle) noexcept;
matrix4x4 perspective(float fov_y, float aspect, float near_plane, float far_plane);

} // namespace e3d::math
