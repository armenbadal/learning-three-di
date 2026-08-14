module e3d.renderer;

import std;
import e3d.math;

namespace e3d::renderer {

screen_vertex viewport_transform(const math::vector3& ndc, float width, float height) noexcept
{
    return {
        .position = {
            (ndc.x() + 1.0F) * 0.5F * width,
            (1.0F - ndc.y()) * 0.5F * height
        },
        .depth = (ndc.z() + 1.0F) * 0.5F
    };
}

math::vector3 perspective_divide(const math::vector4& p)
{
    return {
        p.x() / p.w(),
        p.y() / p.w(),
        p.z() / p.w()
    };
}

} // namespace e3d::renderer
