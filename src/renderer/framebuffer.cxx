#include "engine3d/renderer/framebuffer.hxx"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace e3d::renderer {

namespace {
    std::size_t checked_area(std::size_t width, std::size_t height)
    {
        const auto max_dimension = static_cast<std::size_t>(std::numeric_limits<int>::max());
        if( width > max_dimension || height > max_dimension )
            throw std::invalid_argument("framebuffer dimensions too large");

        if( height != 0 && width > std::numeric_limits<std::size_t>::max() / height )
            throw std::invalid_argument("framebuffer dimensions too large");

        return width * height;
    }
}

framebuffer::framebuffer(std::size_t width, std::size_t height)
    : _width{width}
    , _height{height}
    , _pixels(checked_area(width, height), graphics::black)
{
}

graphics::colour& framebuffer::at(std::size_t x, std::size_t y)
{
    return const_cast<graphics::colour&>(static_cast<const framebuffer*>(this)->at(x, y));
}

const graphics::colour& framebuffer::at(std::size_t x, std::size_t y) const
{
    if( x >= _width || y >= _height )
        throw std::out_of_range("framebuffer index out of range");

    return _pixels[y * _width + x];
}

graphics::colour& framebuffer::operator()(std::size_t x, std::size_t y) noexcept
{
    return _pixels[y * _width + x];
}

const graphics::colour& framebuffer::operator()(std::size_t x, std::size_t y) const noexcept
{
    return _pixels[y * _width + x];
}

void framebuffer::set_clipped(int x, int y, graphics::colour p) noexcept
{
    if( x < 0 || y < 0 )
        return;

    const auto ux = static_cast<std::size_t>(x);
    const auto uy = static_cast<std::size_t>(y);
    if( ux >= _width || uy >= _height )
        return;

    _pixels[uy * _width + ux] = p;
}

void framebuffer::clear(graphics::colour c) noexcept
{
    std::fill(_pixels.begin(), _pixels.end(), c);
}

} // namespace e3d::renderer
