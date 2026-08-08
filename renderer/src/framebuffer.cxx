#include "framebuffer.hxx"

#include <algorithm>

namespace renderer {

framebuffer::framebuffer(unsigned int width, unsigned int height)
    : _width{width}
    , _height{height}
    , _pixels(width * height, black)
{
}

colour framebuffer::get(int x, int y) const
{
    if( x >= _width || y >= _height )
        return white;

    return _pixels[y * _width + x];
}

void framebuffer::set(int x, int y, colour p)
{
    if( x < 0 || y < 0 )
        return;
 
    if( x >= _width || y >= _height )
        return;

    _pixels[y * _width + x] = p;
}

void framebuffer::clear(colour c)
{
    std::fill(_pixels.begin(), _pixels.end(), c);
}

} // namespace renderer