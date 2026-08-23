#include "vertex_array.hxx"

#include <utility>

namespace e3d::renderer::opengl {

vertex_array::vertex_array()
{
    glGenVertexArrays(1, &_id);
}

vertex_array::~vertex_array()
{
    if( _id != 0 )
        glDeleteVertexArrays(1, &_id);
}

vertex_array::vertex_array(vertex_array&& other) noexcept
    : _id{std::exchange(other._id, 0)}
{}

vertex_array& vertex_array::operator=(vertex_array&& other) noexcept
{
    if( this == &other )
        return *this;

    if( _id != 0 )
        glDeleteVertexArrays(1, &_id);
    _id = std::exchange(other._id, 0);
    return *this;
}

void vertex_array::bind() const noexcept
{
    glBindVertexArray(_id);
}

void vertex_array::unbind() noexcept
{
    glBindVertexArray(0);
}

} // namespace e3d::renderer::opengl
