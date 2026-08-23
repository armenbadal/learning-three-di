#include "engine3d/renderer/software/pipeline.hxx"

#include "engine3d/renderer/software/clipping.hxx"

namespace e3d::renderer {

pipeline::pipeline(framebuffer& fb)
    : _framebuffer{fb}
    , _rasterizer{fb}
{}

std::vector<screen_triangle> pipeline::transform_triangle(
    const triangle3d& triangle,
    const math::matrix4x4& model,
    const camera::camera& camera) const
{
    if( _framebuffer.width() == 0 || _framebuffer.height() == 0 )
        return {};

    const float width = static_cast<float>(_framebuffer.width());
    const float height = static_cast<float>(_framebuffer.height());
    const float aspect = width / height;
    const auto transform = camera.projection_matrix(aspect) * camera.view_matrix() * model;

    const auto to_clip_vertex = [&transform](const auto& vertex) {
        return clip_vertex{
            .position = transform * math::vector4{vertex.position, 1.0F},
            .colour = graphics::to_colourf(vertex.colour)
        };
    };
    const std::array clip_triangle{
        to_clip_vertex(triangle[0]),
        to_clip_vertex(triangle[1]),
        to_clip_vertex(triangle[2])
    };

    const auto clipped = clip_polygon(clip_triangle);
    if( clipped.size() < 3 )
        return {};

    std::vector<screen_vertex> polygon;
    polygon.reserve(clipped.size());
    for( const auto& vertex : clipped ) {
        const auto ndc = perspective_divide(vertex.position);
        auto screen = viewport_transform(ndc, width, height);
        screen.inv_w = 1.0F / vertex.position.w();
        screen.colour_over_w = vertex.colour * screen.inv_w;
        polygon.push_back(screen);
    }

    std::vector<screen_triangle> triangles;
    triangles.reserve(polygon.size() - 2);
    for( std::size_t i = 1; i + 1 < polygon.size(); ++i )
        triangles.push_back({polygon[0], polygon[i], polygon[i + 1]});

    return triangles;
}

void pipeline::draw_filled_triangle(
    const triangle3d& triangle,
    const math::matrix4x4& model,
    const camera::camera& camera,
    graphics::colour colour)
{
    for( const auto& screen : transform_triangle(triangle, model, camera) )
        _rasterizer.draw_filled_triangle(screen[0], screen[1], screen[2], colour);
}

void pipeline::draw_filled_triangle(
    const triangle3d& triangle,
    const math::matrix4x4& model,
    const camera::camera& camera)
{
    for( const auto& screen : transform_triangle(triangle, model, camera) )
        _rasterizer.draw_filled_triangle(screen[0], screen[1], screen[2]);
}

} // namespace e3d::renderer
