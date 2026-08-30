#include "buffer.hxx"

#include <utility>

namespace e3d::renderer::opengl {

vertex_buffer::vertex_buffer()
{
    glGenBuffers(1, &_id);
}

vertex_buffer::vertex_buffer(std::span<const geometry::vertex> vertices)
    : vertex_buffer{}
{
    upload(vertices);
}

void vertex_buffer::upload(std::span<const geometry::vertex> vertices) const noexcept
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(), GL_STATIC_DRAW);
}

vertex_buffer::~vertex_buffer()
{
    if( _id != 0 )
        glDeleteBuffers(1, &_id);
}

vertex_buffer::vertex_buffer(vertex_buffer&& other) noexcept
    : _id{std::exchange(other._id, 0)}
{}

vertex_buffer& vertex_buffer::operator=(vertex_buffer&& other) noexcept
{
    if( this == &other )
        return *this;

    if( _id != 0 )
        glDeleteBuffers(1, &_id);
    _id = std::exchange(other._id, 0);
    return *this;
}

void vertex_buffer::bind() const noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void vertex_buffer::unbind() noexcept
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

index_buffer::index_buffer()
{
    glGenBuffers(1, &_id);
}

index_buffer::index_buffer(std::span<const std::uint32_t> indices)
    : index_buffer{}
{
    upload(indices);
}

void index_buffer::upload(std::span<const std::uint32_t> indices) const noexcept
{
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size_bytes()), indices.data(), GL_STATIC_DRAW);
}

index_buffer::~index_buffer()
{
    if( _id != 0 )
        glDeleteBuffers(1, &_id);
}

index_buffer::index_buffer(index_buffer&& other) noexcept
    : _id{std::exchange(other._id, 0)}
{}

index_buffer& index_buffer::operator=(index_buffer&& other) noexcept
{
    if( this == &other )
        return *this;

    if( _id != 0 )
        glDeleteBuffers(1, &_id);
    _id = std::exchange(other._id, 0);
    return *this;
}

void index_buffer::bind() const noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void index_buffer::unbind() noexcept
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace e3d::renderer::opengl
