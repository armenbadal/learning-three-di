#pragma once

#include "matrix4x4.hxx"
#include "vector3.hxx"

namespace math {

class transform {
public:
    // Rotation components are radians. model_matrix() applies scale, rotation, then translation.
    transform(vector3 tr, vector3 rt, vector3 sc) noexcept;

    matrix4x4 model_matrix() const noexcept;

private:
    vector3 _translation;
    vector3 _rotation;
    vector3 _scaling;
};

} // namespace math
