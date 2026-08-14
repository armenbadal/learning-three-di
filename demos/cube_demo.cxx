import e3d;

namespace camera = e3d::camera;
namespace graphics = e3d::graphics;
namespace math = e3d::math;
namespace renderer = e3d::renderer;

namespace {

struct cube_face {
    std::array<std::size_t, 4> indices;
    graphics::colour colour;
};

} // namespace

int main()
{
    renderer::framebuffer framebuffer{640, 480};
    framebuffer.clear({18, 22, 32});
    renderer::pipeline pipeline{framebuffer};

    const camera::camera camera{
        ._position = {0.0F, 0.0F, 5.0F},
        ._target = {0.0F, 0.0F, 0.0F},
        ._up = {0.0F, 1.0F, 0.0F},
        ._fov_y = std::numbers::pi_v<float> / 3.0F,
        ._near_plane = 0.1F,
        ._far_plane = 100.0F
    };

    const std::array vertices{
        math::vector3{-1.0F, -1.0F, -1.0F},
        math::vector3{1.0F, -1.0F, -1.0F},
        math::vector3{1.0F, 1.0F, -1.0F},
        math::vector3{-1.0F, 1.0F, -1.0F},
        math::vector3{-1.0F, -1.0F, 1.0F},
        math::vector3{1.0F, -1.0F, 1.0F},
        math::vector3{1.0F, 1.0F, 1.0F},
        math::vector3{-1.0F, 1.0F, 1.0F}
    };

    const std::array faces{
        cube_face{{4, 5, 6, 7}, {235, 70, 70}},
        cube_face{{1, 0, 3, 2}, {120, 45, 160}},
        cube_face{{0, 4, 7, 3}, {70, 120, 235}},
        cube_face{{5, 1, 2, 6}, {70, 210, 120}},
        cube_face{{3, 7, 6, 2}, {245, 190, 55}},
        cube_face{{0, 1, 5, 4}, {70, 210, 220}}
    };

    const auto model = math::model_matrix(
        {0.0F, 0.0F, 0.0F},
        {0.45F, 0.65F, 0.12F},
        {1.0F, 1.0F, 1.0F});

    for( const auto& face : faces ) {
        const auto& i = face.indices;
        const renderer::triangle3d first{{
            {vertices[i[0]]},
            {vertices[i[1]]},
            {vertices[i[2]]}
        }};
        const renderer::triangle3d second{{
            {vertices[i[0]]},
            {vertices[i[2]]},
            {vertices[i[3]]}
        }};

        pipeline.draw_filled_triangle(first, model, camera, face.colour);
        pipeline.draw_filled_triangle(second, model, camera, face.colour);
    }

    renderer::save_as_ppm(framebuffer, "cube_demo.ppm");
    return 0;
}
