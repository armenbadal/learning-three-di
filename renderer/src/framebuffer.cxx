#include "framebuffer.hxx"

namespace renderer {

framebuffer::framebuffer(unsigned int width, unsigned int height)
    : _width{width}
    , _height{height}
    , _colours(width * height, white)
{
}

colour framebuffer::get(unsigned int x, unsigned int y) const
{
    if( x >= _width || y >= _height )
        return white;

    return _colours[y * _width + x];
}

void framebuffer::set(unsigned int x, unsigned int y, colour p)
{
    if( x >= _width || y >= _height )
        return;

    _colours[y * _width + x] = p;
}

} // namespace renderer