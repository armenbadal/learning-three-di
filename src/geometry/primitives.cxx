#include "engine3d/geometry/primitives.hxx"

#include <cstdint>
#include <vector>

namespace e3d::geometry::primitives {

mesh cube(float size)
{
    return cube(size, {
        graphics::white,
        graphics::white,
        graphics::white,
        graphics::white,
        graphics::white,
        graphics::white,
        graphics::white,
        graphics::white,
    });
}

mesh cube(float size, const std::array<graphics::colour, 8>& colors)
{
    const float half_size = size * 0.5F;

    return {
        std::vector<vertex>{
            {{-half_size, -half_size, -half_size}, colors[0]},
            {{half_size, -half_size, -half_size}, colors[1]},
            {{half_size, half_size, -half_size}, colors[2]},
            {{-half_size, half_size, -half_size}, colors[3]},
            {{-half_size, -half_size, half_size}, colors[4]},
            {{half_size, -half_size, half_size}, colors[5]},
            {{half_size, half_size, half_size}, colors[6]},
            {{-half_size, half_size, half_size}, colors[7]},
        },
        std::vector<std::uint32_t>{
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            0, 4, 5, 0, 5, 1,
            3, 2, 6, 3, 6, 7,
            1, 5, 6, 1, 6, 2,
            0, 3, 7, 0, 7, 4,
        }};
}

mesh plane(float width, float depth)
{
    const float half_width = width * 0.5F;
    const float half_depth = depth * 0.5F;

    return {
        std::vector<vertex>{
            {{-half_width, 0.0F, -half_depth}},
            {{half_width, 0.0F, -half_depth}},
            {{half_width, 0.0F, half_depth}},
            {{-half_width, 0.0F, half_depth}},
        },
        std::vector<std::uint32_t>{
            0, 2, 1,
            0, 3, 2,
        }};
}

} // namespace e3d::geometry::primitives
