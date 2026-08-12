#include "engine3d/renderer/framebuffer.hxx"
#include "engine3d/renderer/rasterizer.hxx"
#include "engine3d/renderer/imager.hxx"
#include "engine3d/geometry/vertex.hxx"
#include "engine3d/math/vector2.hxx"

namespace renderer = engine3d::renderer;
namespace geometry = engine3d::geometry;
namespace math = engine3d::math;

void example_rectangle()
{
    renderer::framebuffer fb{40, 20};
    fb.clear(renderer::white);
    renderer::rasterizer r{fb};
    r.draw_line({0.0F, 0.0F}, {39.0F, 19.0F}, renderer::black);
    r.draw_line({0.0F, 0.0F}, {39.0F, 0.0F}, renderer::black);
    r.draw_line({0.0F, 0.0F}, {0.0F, 19.0F}, renderer::black);
    r.draw_line({0.0F, 19.0F}, {39.0F, 19.0F}, renderer::black);
    r.draw_line({39.0F, 0.0F}, {39.0F, 19.0F}, renderer::black);
    renderer::save_as_ppm(fb, "line.ppm");
}

void example_triangle()
{
    renderer::framebuffer fb{40, 40};
    fb.clear(renderer::white);
    renderer::rasterizer r{fb};
    r.draw_triangle(
        {20.0F, 5.0},
        {5.0F, 35.0F},
        {35.0F, 35.0F},
        {255, 0, 0}
    );
    renderer::save_as_ppm(fb, "triangle.ppm");
}

void example_filled_triangle()
{
    renderer::framebuffer fb{40, 40};
    fb.clear(renderer::white);
    renderer::rasterizer r{fb};
    r.draw_filled_triangle(
        {20.0F, 5.0},
        {5.0F, 35.0F},
        {35.0F, 35.0F},
        {255, 0, 0}
    );
    renderer::save_as_ppm(fb, "filled-triangle.ppm");
}

void example_coloured_triangle()
{
    renderer::framebuffer fb{40, 40};
    fb.clear(renderer::white);
    renderer::rasterizer r{fb};

    geometry::vertex2d v0{{20.0F, 5.0}, {255, 0, 0}};
    geometry::vertex2d v1{{5.0F, 35.0F}, {0, 255, 0}};
    geometry::vertex2d v2{{35.0F, 35.0F}, {0, 0, 255}};
    r.draw_filled_triangle(v0, v1, v2);

    renderer::save_as_ppm(fb, "coloured-triangle.ppm");
}

void example_rectangle_with_triangles()
{
    renderer::framebuffer fb{40, 40};
    fb.clear(renderer::white);
    renderer::rasterizer r{fb};

    math::vector2 p0{4.0F, 4.0F};
    math::vector2 p1{34.0F, 4.0F};
    math::vector2 p2{34.0F, 34.0F};
    math::vector2 p3{4.0F, 34.0F};

    geometry::vertex2d v0{p0, {255, 0, 0}};
    geometry::vertex2d v1{p1, {0, 255, 0}};
    geometry::vertex2d v2{p2, {255, 0, 0}};
    geometry::vertex2d v3{p3, {0, 0, 255}};

    r.draw_filled_triangle(v0, v1, v3);
    r.draw_filled_triangle(v1, v2, v3);

    renderer::save_as_ppm(fb, "rectangle.ppm");
}

int main()
{
    example_rectangle();
    example_triangle();
    example_filled_triangle();
    example_coloured_triangle();
    example_rectangle_with_triangles();
    return 0;
}
