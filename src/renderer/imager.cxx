module e3d.renderer;

import std;

namespace e3d::renderer {

void save_as_ppm(const framebuffer& fb, std::filesystem::path p)
{
    auto fout = std::ofstream{p};
    if( !fout )
        throw std::runtime_error("failed to open ppm file");

    const auto w = fb.width(), h = fb.height();

    fout << "P3\n";
    fout << w << ' ' << h << '\n';
    fout << 255 << '\n';

    for( std::size_t r = 0; r < h; ++r ) {
        for( std::size_t c = 0; c < w; ++c ) {
            const auto px = fb(c, r);
            fout << static_cast<unsigned int>(px.r()) << ' '
                 << static_cast<unsigned int>(px.g()) << ' '
                 << static_cast<unsigned int>(px.b()) << ' ';
        }
        fout << '\n';
    }
    fout << '\n';

    fout.close();
    if( !fout )
        throw std::runtime_error("failed to write ppm file");
}

} // namespace e3d::renderer
