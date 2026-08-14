import e3d;

namespace camera = e3d::camera;
namespace graphics = e3d::graphics;
namespace math = e3d::math;
namespace renderer = e3d::renderer;

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

    const renderer::triangle3d triangle{{
        {{-1.2F, -1.0F, 0.0F}, {255, 70, 70}},
        {{1.2F, -1.0F, 0.0F}, {70, 255, 120}},
        {{0.0F, 1.2F, 0.0F}, {80, 120, 255}}
    }};

    const auto near_model = math::model_matrix(
        {0.2F, 0.0F, 0.0F},
        {0.15F, -0.25F, 0.18F},
        {1.0F, 1.0F, 1.0F});
    const auto far_model = math::model_matrix(
        {-0.25F, 0.05F, -1.5F},
        {-0.1F, 0.2F, -0.15F},
        {1.35F, 1.35F, 1.35F});

    pipeline.draw_filled_triangle(triangle, near_model, camera);
    pipeline.draw_filled_triangle(triangle, far_model, camera, {235, 185, 55});

    renderer::save_as_ppm(framebuffer, "3d_pipeline_demo.ppm");
    return 0;
}
