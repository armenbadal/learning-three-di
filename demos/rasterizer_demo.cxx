#include "framebuffer.hxx"
#include "rasterizer.hxx"
#include "utility.hxx"

int main()
{
    renderer::framebuffer fb{40, 20};
    renderer::rasterizer r{fb};
    r.draw_line(0.0F, 0.0F, 39.0F, 19.0F, renderer::black);
    r.draw_line(0.0F, 0.0F, 39.0F, 0.0F, renderer::black);
    r.draw_line(0.0F, 0.0F, 0.0F, 19.0F, renderer::black);
    r.draw_line(0.0F, 19.0F, 39.0F, 19.0F, renderer::black);
    r.draw_line(39.0F, 0.0F, 39.0F, 19.0F, renderer::black);
    renderer::save_as_ppm(fb, "line.ppm");
    return 0;
}