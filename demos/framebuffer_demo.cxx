#include "engine3d/graphics/colour.hxx"
#include "engine3d/renderer/framebuffer.hxx"
#include "engine3d/renderer/imager.hxx"

#include <cstddef>

namespace renderer = e3d::renderer;
namespace graphics = e3d::graphics;

int main()
{
    renderer::framebuffer fb{40, 20};
    for(std::size_t i = 0; i < 20; ++i) {
        fb(2*i, i) = graphics::black;
        fb(2*i+1, i) = graphics::black;
    }

    renderer::save_as_ppm(fb, "empty.ppm");
    return 0;
}
