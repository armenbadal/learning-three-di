#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <numbers>

import e3d;

namespace camera = e3d::camera;
namespace graphics = e3d::graphics;
namespace math = e3d::math;
namespace renderer = e3d::renderer;

namespace {

auto approx(float value)
{
    return Catch::Approx(value).margin(1e-4F);
}

camera::camera forward_camera()
{
    return {
        ._position = {0.0F, 0.0F, 0.0F},
        ._target = {0.0F, 0.0F, -1.0F},
        ._up = {0.0F, 1.0F, 0.0F},
        ._fov_y = std::numbers::pi_v<float> / 2.0F,
        ._near_plane = 1.0F,
        ._far_plane = 20.0F
    };
}

std::size_t count_colour(const renderer::framebuffer& fb, graphics::colour colour)
{
    return static_cast<std::size_t>(std::ranges::count(fb.pixels(), colour));
}

std::size_t count_drawn(const renderer::framebuffer& fb, graphics::colour clear_colour)
{
    return fb.pixels().size() - count_colour(fb, clear_colour);
}

renderer::triangle3d triangle_at_depth(float z, float scale = 1.0F)
{
    return {{
        {{-0.8F * scale, -0.8F * scale, z}},
        {{0.8F * scale, -0.8F * scale, z}},
        {{0.0F, 0.8F * scale, z}}
    }};
}

} // namespace

TEST_CASE("3D pipeline renders a model-space triangle through every stage")
{
    renderer::framebuffer fb{100, 100};
    fb.clear(graphics::white);
    renderer::pipeline pipeline{fb};
    const auto triangle = triangle_at_depth(-3.0F);
    const auto model = math::model_matrix(
        {0.5F, 0.0F, 0.0F},
        {0.0F, 0.0F, 0.0F},
        {1.0F, 1.0F, 1.0F});
    const graphics::colour fill{220, 40, 30};

    const auto screen = pipeline.transform_triangle(triangle, model, forward_camera());
    REQUIRE(screen.size() == 1);
    CHECK(screen[0][0].position.x() == approx(45.0F));
    CHECK(screen[0][1].position.x() == approx(71.6667F));
    CHECK(screen[0][2].position.x() == approx(58.3333F));

    pipeline.draw_filled_triangle(triangle, model, forward_camera(), fill);

    CHECK(fb.at(58, 55) == fill);
    CHECK(fb.depth_at(58, 55) < 1.0F);
    CHECK(count_drawn(fb, graphics::white) > 0);
}

TEST_CASE("3D pipeline composes projection view and model in column-vector order")
{
    renderer::framebuffer fb{120, 120};
    fb.clear(graphics::white);
    renderer::pipeline pipeline{fb};
    auto camera = forward_camera();
    camera._position = {0.0F, 0.0F, 1.0F};
    camera._target = {0.0F, 0.0F, 0.0F};
    const auto model = math::model_matrix(
        {0.0F, 0.0F, 0.0F},
        {0.0F, 0.0F, 0.0F},
        {1.0F, 1.0F, 2.0F});
    const renderer::triangle3d triangle{{
        {{0.6F, -0.3F, -1.0F}},
        {{0.6F, 0.3F, -1.0F}},
        {{0.0F, 0.0F, -1.0F}}
    }};
    const graphics::colour fill{30, 180, 70};

    const auto screen = pipeline.transform_triangle(triangle, model, camera);
    REQUIRE(screen.size() == 1);
    CHECK(screen[0][0].position.x() == approx(72.0F));
    CHECK(screen[0][0].position.y() == approx(66.0F));

    pipeline.draw_filled_triangle(triangle, model, camera, fill);
    CHECK(fb.at(66, 60) == fill);
}

TEST_CASE("3D pipeline rasterizes no fragments for a fully outside primitive")
{
    renderer::framebuffer fb{80, 80};
    fb.clear(graphics::white);
    renderer::pipeline pipeline{fb};
    const renderer::triangle3d triangle{{
        {{5.0F, -0.5F, -2.0F}},
        {{6.0F, 0.0F, -2.0F}},
        {{5.0F, 0.5F, -2.0F}}
    }};

    pipeline.draw_filled_triangle(
        triangle, math::matrix4x4::identity(), forward_camera(), graphics::black);

    CHECK(count_drawn(fb, graphics::white) == 0);
    CHECK(std::ranges::all_of(fb.depths(), [](float depth) { return depth == 1.0F; }));
}

TEST_CASE("3D pipeline clips a crossing primitive and rasterizes its visible polygon")
{
    renderer::framebuffer fb{100, 100};
    fb.clear(graphics::white);
    renderer::pipeline pipeline{fb};
    const renderer::triangle3d triangle{{
        {{-1.0F, -1.0F, -2.0F}},
        {{3.0F, -1.0F, -2.0F}},
        {{-1.0F, 1.0F, -2.0F}}
    }};

    const auto clipped = pipeline.transform_triangle(
        triangle, math::matrix4x4::identity(), forward_camera());
    REQUIRE(clipped.size() == 2);

    pipeline.draw_filled_triangle(
        triangle, math::matrix4x4::identity(), forward_camera(), graphics::black);

    CHECK(count_drawn(fb, graphics::white) > 0);
    CHECK(fb.at(50, 50) == graphics::black);
}

TEST_CASE("3D pipeline depth testing makes overlapping geometry draw-order independent")
{
    renderer::framebuffer near_first{100, 100};
    renderer::framebuffer far_first{100, 100};
    near_first.clear(graphics::white);
    far_first.clear(graphics::white);
    renderer::pipeline a{near_first};
    renderer::pipeline b{far_first};
    const auto near_triangle = triangle_at_depth(-2.0F);
    const auto far_triangle = triangle_at_depth(-4.0F, 2.0F);
    const graphics::colour near_colour{230, 40, 40};
    const graphics::colour far_colour{40, 60, 230};
    const auto identity = math::matrix4x4::identity();
    const auto camera = forward_camera();

    a.draw_filled_triangle(near_triangle, identity, camera, near_colour);
    a.draw_filled_triangle(far_triangle, identity, camera, far_colour);
    b.draw_filled_triangle(far_triangle, identity, camera, far_colour);
    b.draw_filled_triangle(near_triangle, identity, camera, near_colour);

    CHECK(std::ranges::equal(near_first.pixels(), far_first.pixels()));
    CHECK(std::ranges::equal(near_first.depths(), far_first.depths()));
    CHECK(near_first.at(50, 50) == near_colour);
}

TEST_CASE("3D pipeline perspective projection makes farther geometry smaller")
{
    renderer::framebuffer near_fb{100, 100};
    renderer::framebuffer far_fb{100, 100};
    near_fb.clear(graphics::white);
    far_fb.clear(graphics::white);
    renderer::pipeline near_pipeline{near_fb};
    renderer::pipeline far_pipeline{far_fb};
    const auto triangle = triangle_at_depth(0.0F);
    const auto near_model = math::model_matrix(
        {0.0F, 0.0F, -2.0F}, {0.0F, 0.0F, 0.0F}, {1.0F, 1.0F, 1.0F});
    const auto far_model = math::model_matrix(
        {0.0F, 0.0F, -4.0F}, {0.0F, 0.0F, 0.0F}, {1.0F, 1.0F, 1.0F});
    const auto camera = forward_camera();

    near_pipeline.draw_filled_triangle(triangle, near_model, camera, graphics::black);
    far_pipeline.draw_filled_triangle(triangle, far_model, camera, graphics::black);

    const auto near_pixels = count_drawn(near_fb, graphics::white);
    const auto far_pixels = count_drawn(far_fb, graphics::white);
    CHECK(near_pixels > far_pixels);
    CHECK(far_pixels > 0);
}

TEST_CASE("3D pipeline applies camera movement through the view matrix")
{
    renderer::framebuffer centred_fb{100, 100};
    renderer::framebuffer shifted_fb{100, 100};
    centred_fb.clear(graphics::white);
    shifted_fb.clear(graphics::white);
    renderer::pipeline centred_pipeline{centred_fb};
    renderer::pipeline shifted_pipeline{shifted_fb};
    const auto triangle = triangle_at_depth(0.0F);
    const camera::camera centred_camera{
        ._position = {0.0F, 0.0F, 5.0F},
        ._target = {0.0F, 0.0F, 0.0F},
        ._fov_y = std::numbers::pi_v<float> / 2.0F,
        ._near_plane = 1.0F,
        ._far_plane = 20.0F
    };
    const camera::camera shifted_camera{
        ._position = {1.0F, 0.0F, 5.0F},
        ._target = {1.0F, 0.0F, 4.0F},
        ._fov_y = std::numbers::pi_v<float> / 2.0F,
        ._near_plane = 1.0F,
        ._far_plane = 20.0F
    };
    const auto identity = math::matrix4x4::identity();

    const auto centred = centred_pipeline.transform_triangle(triangle, identity, centred_camera);
    const auto shifted = shifted_pipeline.transform_triangle(triangle, identity, shifted_camera);
    REQUIRE(centred.size() == 1);
    REQUIRE(shifted.size() == 1);
    CHECK(centred[0][2].position.x() == approx(50.0F));
    CHECK(shifted[0][2].position.x() == approx(40.0F));

    centred_pipeline.draw_filled_triangle(triangle, identity, centred_camera, graphics::black);
    shifted_pipeline.draw_filled_triangle(triangle, identity, shifted_camera, graphics::black);
    CHECK(centred_fb.at(50, 50) == graphics::black);
    CHECK(shifted_fb.at(40, 50) == graphics::black);
    CHECK(shifted_fb.at(55, 50) == graphics::white);
}
