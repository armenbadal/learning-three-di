#include <engine3d/renderer/colour.hxx>
#include <engine3d/renderer/framebuffer.hxx>
#include <engine3d/renderer/utility.hxx>

#include <cstddef>

namespace renderer = engine3d::renderer;

int main()
{
    renderer::framebuffer fb{40, 20};
    for(std::size_t i = 0; i < 20; ++i) {
        fb(2*i, i) = renderer::black;
        fb(2*i+1, i) = renderer::black;
    }

    renderer::save_as_ppm(fb, "empty.ppm");
    return 0;
}
