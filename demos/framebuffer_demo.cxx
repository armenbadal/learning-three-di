#include "framebuffer.hxx"
#include "utility.hxx"
#include "colour.hxx"

int main()
{
    renderer::framebuffer fb{40, 20};
    for(int i = 0; i < 20; ++i) {
        fb.set(2*i, i, renderer::black);
        fb.set(2*i+1, i, renderer::black);
    }

    renderer::save_as_ppm(fb, "empty.ppm");
    return 0;
}
