#include "engine3d/camera/camera.hxx"
#include "engine3d/geometry/mesh.hxx"
#include "engine3d/renderer/software/imager.hxx"
#include "engine3d/renderer/software/software_renderer.hxx"
#include "engine3d/scene/scene.hxx"

#include <memory>
#include <numbers>
#include <vector>

namespace camera = e3d::camera;
namespace geometry = e3d::geometry;
namespace math = e3d::math;
namespace renderer = e3d::renderer;
namespace scene = e3d::scene;

int main()
{
    const auto cube = std::make_shared<const geometry::mesh>(
        std::vector<geometry::vertex>{
            {{-1.0F, -1.0F, -1.0F}, {235, 70, 70}},
            {{1.0F, -1.0F, -1.0F}, {70, 210, 120}},
            {{1.0F, 1.0F, -1.0F}, {70, 120, 235}},
            {{-1.0F, 1.0F, -1.0F}, {245, 190, 55}},
            {{-1.0F, -1.0F, 1.0F}, {220, 80, 180}},
            {{1.0F, -1.0F, 1.0F}, {70, 210, 220}},
            {{1.0F, 1.0F, 1.0F}, {235, 235, 235}},
            {{-1.0F, 1.0F, 1.0F}, {145, 95, 235}},
        },
        std::vector<std::uint32_t>{
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            0, 4, 5, 0, 5, 1,
            3, 2, 6, 3, 6, 7,
            1, 5, 6, 1, 6, 2,
            0, 3, 7, 0, 7, 4,
        });

    scene::scene world;
    world.add(scene::scene_object{cube, scene::transform{{0.0F, 0.0F, 0.0F}, {0.45F, 0.65F, 0.12F}}});

    const camera::camera view{
        {0.0F, 0.0F, 5.0F},
        {0.0F, 0.0F, 0.0F},
        camera::perspective_projection{
            .fov_y = std::numbers::pi_v<float> / 3.0F,
            .near_plane = 0.1F,
            .far_plane = 100.0F,
        }};

    renderer::software_renderer software_renderer{640, 480};
    renderer::render_settings settings{ .clear_colour = {18, 22, 32} };
    software_renderer.render(world, view, settings);

    renderer::save_as_ppm(software_renderer.framebuffer(), "software_renderer_demo.ppm");
    return 0;
}
