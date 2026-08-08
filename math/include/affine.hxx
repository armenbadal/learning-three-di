#pragma once

#include "matrix4x4.hxx"
#include "vector3.hxx"

namespace math {

class transform {
public:
    transform(const vector3& tr, const vector3& rt, const vector3& sc);

    matrix4x4 model_matrix() const;

private:
    vector3 _translation;
    vector3 _rotation;
    vector3 _scaling;
};

} // namespace math

