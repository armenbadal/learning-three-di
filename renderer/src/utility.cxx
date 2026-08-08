#include "utility.hxx"

#include <fstream>

namespace renderer {

void save_as_ppm(const framebuffer& fb, std::filesystem::path p)
{
    if( auto fout = std::ofstream{p}; fout ) {
        const auto w = fb.width(), h = fb.height();

        fout << "P3\n";
        fout << w << ' ' << h << '\n';
        fout << 255 << '\n';

        for( unsigned int r = 0; r < h; ++r ) {
            for( unsigned int c = 0; c < w; ++c ) {
                auto px = fb.get(c, r);
                fout << static_cast<unsigned int>(px.r()) << ' '
                     << static_cast<unsigned int>(px.g()) << ' '
                     << static_cast<unsigned int>(px.b()) << ' ';
            }
            fout << '\n';
        }
        fout << '\n';
    }
}

} // namespace renderer
