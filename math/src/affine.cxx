#include "affine.hxx"

namespace math {

transform::transform(const vector3& tr, const vector3& rt, const vector3& sc)
    : _translation{tr}, _rotation{rt}, _scaling{sc}
{}


matrix4x4 transform::model_matrix() const
{   
    auto translation = matrix4x4::translation(_translation);

    auto rot_x = matrix4x4::rotation_x(_rotation.x());
    auto rot_y = matrix4x4::rotation_y(_rotation.y());
    auto rot_z = matrix4x4::rotation_z(_rotation.z());
    auto rotation = rot_z * rot_y * rot_x;

    auto scaling = matrix4x4::scaling(_scaling);

    return translation * rotation * scaling;
}

} // namespace math
