#include "framebuffer.hxx"
#include "rasterizer.hxx"
#include "utility.hxx"
#include "vector2.hxx"
#include "vertex2d.hxx"

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

    renderer::vertex2d v0{{20.0F, 5.0}, {255, 0, 0}};
    renderer::vertex2d v1{{5.0F, 35.0F}, {0, 255, 0}};
    renderer::vertex2d v2{{35.0F, 35.0F}, {0, 0, 255}};
    r.draw_filled_triangle(v0, v1, v2);

    renderer::save_as_ppm(fb, "coloured-triangle.ppm");
}

int main()
{
    example_rectangle();
    example_triangle();
    example_filled_triangle();
    example_coloured_triangle();
    return 0;
}