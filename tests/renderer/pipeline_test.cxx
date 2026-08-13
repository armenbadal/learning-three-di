#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "engine3d/math/transform.hxx"
#include "engine3d/renderer/pipeline.hxx"

#include <numbers>

namespace camera = e3d::camera;
namespace graphics = e3d::graphics;
namespace math = e3d::math;
namespace renderer = e3d::renderer;

namespace {

auto approx(float value)
{
    return Catch::Approx(value).margin(1e-4F);
}

camera::camera test_camera()
{
    return {
        ._position = {0.0F, 0.0F, 0.0F},
        ._target = {0.0F, 0.0F, -1.0F},
        ._fov_y = std::numbers::pi_v<float> / 2.0F,
        ._near_plane = 1.0F,
        ._far_plane = 10.0F
    };
}

void check_position(const renderer::screen_vertex& vertex, float x, float y)
{
    CHECK(vertex.position.x() == approx(x));
    CHECK(vertex.position.y() == approx(y));
}

} // namespace

TEST_CASE("pipeline transforms a triangle inside the frustum to screen space")
{
    renderer::framebuffer fb{100, 100};
    renderer::pipeline pipeline{fb};
    const renderer::triangle3d triangle{{
        {{-0.5F, -0.5F, -2.0F}},
        {{0.5F, -0.5F, -2.0F}},
        {{0.0F, 0.5F, -2.0F}}
    }};

    const auto result = pipeline.transform_triangle(triangle, math::matrix4x4::identity(), test_camera());

    REQUIRE(result.size() == 1);
    check_position(result[0][0], 37.5F, 62.5F);
    check_position(result[0][1], 62.5F, 62.5F);
    check_position(result[0][2], 50.0F, 37.5F);
    for( const auto& vertex : result[0] ) {
        CHECK(vertex.depth == approx(5.0F / 9.0F));
        CHECK(vertex.inv_w == approx(0.5F));
        CHECK(vertex.colour_over_w == graphics::colourf{127.5F, 127.5F, 127.5F, 127.5F});
    }
}

TEST_CASE("pipeline discards a triangle outside a clip plane")
{
    renderer::framebuffer fb{100, 100};
    renderer::pipeline pipeline{fb};
    const renderer::triangle3d triangle{{
        {{3.0F, -0.5F, -2.0F}},
        {{4.0F, 0.0F, -2.0F}},
        {{3.0F, 0.5F, -2.0F}}
    }};

    CHECK(pipeline.transform_triangle(
        triangle, math::matrix4x4::identity(), test_camera()).empty());
}

TEST_CASE("pipeline clips and fan-triangulates a triangle crossing a clip plane")
{
    renderer::framebuffer fb{100, 100};
    renderer::pipeline pipeline{fb};
    const renderer::triangle3d triangle{{
        {{-1.0F, -1.0F, -2.0F}},
        {{3.0F, -1.0F, -2.0F}},
        {{-1.0F, 1.0F, -2.0F}}
    }};

    const auto result = pipeline.transform_triangle(triangle, math::matrix4x4::identity(), test_camera());

    REQUIRE(result.size() == 2);
    check_position(result[0][0], 25.0F, 75.0F);
    check_position(result[0][1], 100.0F, 75.0F);
    check_position(result[0][2], 100.0F, 62.5F);
    check_position(result[1][0], 25.0F, 75.0F);
    check_position(result[1][1], 100.0F, 62.5F);
    check_position(result[1][2], 25.0F, 25.0F);
}

TEST_CASE("pipeline applies matrices in projection view model order")
{
    renderer::framebuffer fb{120, 120};
    renderer::pipeline pipeline{fb};
    auto camera = test_camera();
    camera._position = {0.0F, 0.0F, 1.0F};
    camera._target = {0.0F, 0.0F, 0.0F};
    const auto model = math::model_matrix(
        {0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 0.0F}, {1.0F, 1.0F, 2.0F});
    const renderer::triangle3d triangle{{
        {{0.6F, -0.3F, -1.0F}},
        {{0.6F, 0.3F, -1.0F}},
        {{0.0F, 0.0F, -1.0F}}
    }};

    const auto result = pipeline.transform_triangle(triangle, model, camera);

    REQUIRE(result.size() == 1);
    check_position(result[0][0], 72.0F, 66.0F);
    check_position(result[0][1], 72.0F, 54.0F);
    check_position(result[0][2], 60.0F, 60.0F);
}

TEST_CASE("pipeline places a triangle for a camera looking at the origin")
{
    renderer::framebuffer fb{200, 100};
    renderer::pipeline pipeline{fb};
    const camera::camera camera{
        ._position = {0.0F, 0.0F, 5.0F},
        ._target = {0.0F, 0.0F, 0.0F},
        ._fov_y = std::numbers::pi_v<float> / 2.0F,
        ._near_plane = 1.0F,
        ._far_plane = 10.0F
    };
    const renderer::triangle3d triangle{{
        {{-1.0F, -1.0F, 0.0F}},
        {{1.0F, -1.0F, 0.0F}},
        {{0.0F, 1.0F, 0.0F}}
    }};

    const auto result = pipeline.transform_triangle(triangle, math::matrix4x4::identity(), camera);

    REQUIRE(result.size() == 1);
    check_position(result[0][0], 90.0F, 60.0F);
    check_position(result[0][1], 110.0F, 60.0F);
    check_position(result[0][2], 100.0F, 40.0F);
}

TEST_CASE("pipeline rasterizes transformed triangles with the requested colour")
{
    renderer::framebuffer fb{20, 20};
    renderer::pipeline pipeline{fb};
    const graphics::colour fill{12, 34, 56};
    const renderer::triangle3d triangle{{
        {{-1.0F, -1.0F, -2.0F}},
        {{1.0F, -1.0F, -2.0F}},
        {{0.0F, 1.0F, -2.0F}}
    }};

    pipeline.draw_filled_triangle(triangle, math::matrix4x4::identity(), test_camera(), fill);

    CHECK(fb.at(10, 10) == fill);
}

TEST_CASE("pipeline perspective-corrects vertex colour across projected depth")
{
    renderer::framebuffer fb{100, 100};
    renderer::pipeline pipeline{fb};
    const renderer::triangle3d triangle{{
        {{-1.0F, -1.0F, -2.0F}, {255, 0, 0}},
        {{1.0F, -1.0F, -2.0F}, {0, 255, 0}},
        {{0.0F, 1.0F, -5.0F}, {0, 0, 255}}
    }};

    pipeline.draw_filled_triangle(triangle, math::matrix4x4::identity(), test_camera());

    CHECK(fb.at(50, 60) == graphics::colour{96, 103, 56});
    CHECK(fb.at(50, 60) != graphics::colour{72, 77, 106});
}
