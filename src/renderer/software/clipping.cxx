#include "engine3d/renderer/software/clipping.hxx"

#include <utility>

namespace e3d::renderer {

clip_vertex interpolate(const clip_vertex& a, const clip_vertex& b, float t) noexcept
{
    return {
        .position = a.position + (b.position - a.position) * t,
        .colour = a.colour + (b.colour - a.colour) * t
    };
}

namespace {

clip_vertex intersect(const clip_vertex& a, const clip_vertex& b, float da, float db) noexcept
{
    return interpolate(a, b, da / (da - db));
}

float distance_to_plane(clip_plane pl, const clip_vertex& v) noexcept
{
    const auto& p = v.position;

    switch( pl ) {
        case clip_plane::left:
            return p.w() + p.x();
        case clip_plane::right:
            return p.w() - p.x();
        case clip_plane::bottom:
            return p.w() + p.y();
        case clip_plane::top:
            return p.w() - p.y();
        case clip_plane::near:
            return p.w() + p.z();
        case clip_plane::far:
            return p.w() - p.z();
    }

    std::unreachable();
}

} // namespace

std::vector<clip_vertex> clip_against_plane(std::span<const clip_vertex> input, clip_plane pl)
{
    std::vector<clip_vertex> output;
    output.reserve(input.size() + 1);

    if( input.empty() )
        return output;

    const auto append = [&output](const clip_vertex& vertex) {
        if( output.empty() || output.back().position != vertex.position )
            output.push_back(vertex);
    };

    auto previous = input.back();
    float dist_previous = distance_to_plane(pl, previous);
    bool previous_inside = dist_previous >= 0.0F;

    for( const auto& current : input ) {
        const float d_current = distance_to_plane(pl, current);
        const bool current_inside = d_current >= 0.0F;

        if( previous_inside && current_inside )
            append(current);
        else if( previous_inside && !current_inside )
            append(intersect(previous, current, dist_previous, d_current));
        else if( !previous_inside && current_inside ) {
            append(intersect(previous, current, dist_previous, d_current));
            append(current);
        }

        previous = current;
        dist_previous = d_current;
        previous_inside = current_inside;
    }

    if( output.size() > 1 && output.front().position == output.back().position )
        output.pop_back();

    return output;
}

std::vector<clip_vertex> clip_polygon(std::span<const clip_vertex> input)
{
    auto polygon = clip_against_plane(input, clip_plane::left);
    polygon = clip_against_plane(polygon, clip_plane::right);
    polygon = clip_against_plane(polygon, clip_plane::bottom);
    polygon = clip_against_plane(polygon, clip_plane::top);
    polygon = clip_against_plane(polygon, clip_plane::near);
    return clip_against_plane(polygon, clip_plane::far);
}

} // namespace e3d::renderer
