#include "gpu_mesh.hxx"

#include <cstddef>

namespace e3d::renderer::opengl {

gpu_mesh::gpu_mesh(const geometry::mesh& mesh)
    : _index_count{mesh.indices().size()}
{
    _vao.bind();
    _vertices.upload(mesh.vertices());
    _indices.upload(mesh.indices());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(geometry::vertex),
        reinterpret_cast<void*>(offsetof(geometry::vertex, position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(geometry::vertex),
        reinterpret_cast<void*>(offsetof(geometry::vertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(geometry::vertex),
        reinterpret_cast<void*>(offsetof(geometry::vertex, colour)));
    glEnableVertexAttribArray(2);

    vertex_array::unbind();
}

void gpu_mesh::draw() const
{
    _vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_index_count), GL_UNSIGNED_INT, nullptr);
}

} // namespace e3d::renderer::opengl
