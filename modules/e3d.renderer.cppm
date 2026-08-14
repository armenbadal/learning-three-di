export module e3d.renderer;

export import e3d.math;
export import e3d.graphics;
export import e3d.geometry;
export import e3d.camera;

import std;

export namespace e3d::renderer {

using geometry::vertex2d;

struct clip_vertex {
    e3d::math::vector4 position;
    graphics::colourf colour{};
};

clip_vertex interpolate(const clip_vertex& a, const clip_vertex& b, float t) noexcept;

enum class clip_plane {
    left,
    right,
    bottom,
    top,
    near,
    far
};

std::vector<clip_vertex> clip_against_plane(std::span<const clip_vertex> input, clip_plane pl);
std::vector<clip_vertex> clip_polygon(std::span<const clip_vertex> input);

struct screen_vertex {
    math::vector2 position;
    float depth;
    float inv_w{1.0F};
    graphics::colourf colour_over_w{};
};

screen_vertex viewport_transform(const math::vector3& ndc, float width, float height) noexcept;
math::vector3 perspective_divide(const math::vector4& p);

class framebuffer {
public:
    framebuffer(std::size_t width, std::size_t height);

    std::size_t width() const noexcept { return _width; }
    std::size_t height() const noexcept { return _height; }

    // Checked access; throws std::out_of_range for coordinates outside the buffer.
    graphics::colour& at(std::size_t x, std::size_t y);
    const graphics::colour& at(std::size_t x, std::size_t y) const;
    float& depth_at(std::size_t x, std::size_t y);
    const float& depth_at(std::size_t x, std::size_t y) const;

    // Unchecked access; caller must keep coordinates in bounds.
    graphics::colour& operator()(std::size_t x, std::size_t y) noexcept;
    const graphics::colour& operator()(std::size_t x, std::size_t y) const noexcept;

    std::span<graphics::colour> pixels() noexcept { return _pixels; }
    std::span<const graphics::colour> pixels() const noexcept { return _pixels; }
    std::span<float> depths() noexcept { return _depth_buffer; }
    std::span<const float> depths() const noexcept { return _depth_buffer; }

    // Clipped write; out-of-bounds coordinates are silently ignored.
    void set_clipped(int x, int y, graphics::colour p) noexcept;

    void clear_colour(graphics::colour c = graphics::black) noexcept;
    void clear_depth() noexcept;
    void clear(graphics::colour c = graphics::black) noexcept;

private:
    std::size_t checked_index(std::size_t x, std::size_t y) const;

    std::size_t _width;
    std::size_t _height;
    std::vector<graphics::colour> _pixels;
    std::vector<float> _depth_buffer;
};

class rasterizer {
public:
    explicit rasterizer(framebuffer& fb);

    void draw_line(screen_vertex v0, screen_vertex v1, graphics::colour c);
    void draw_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, graphics::colour c);
    void draw_filled_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2);
    void draw_filled_triangle(screen_vertex v0, screen_vertex v1, screen_vertex v2, graphics::colour c);
    void draw_filled_triangle(vertex2d v0, vertex2d v1, vertex2d v2);

private:
    void rasterize_filled_triangle(
        screen_vertex v0,
        screen_vertex v1,
        screen_vertex v2,
        const graphics::colour* fill_colour);

    framebuffer& _framebuffer;
};

using triangle3d = std::array<geometry::vertex3d, 3>;
using screen_triangle = std::array<screen_vertex, 3>;

class pipeline {
public:
    explicit pipeline(framebuffer& fb);

    std::vector<screen_triangle> transform_triangle(
        const triangle3d& triangle,
        const math::matrix4x4& model,
        const camera::camera& camera) const;

    void draw_filled_triangle(
        const triangle3d& triangle,
        const math::matrix4x4& model,
        const camera::camera& camera,
        graphics::colour colour);

    void draw_filled_triangle(
        const triangle3d& triangle,
        const math::matrix4x4& model,
        const camera::camera& camera);

private:
    framebuffer& _framebuffer;
    rasterizer _rasterizer;
};

void save_as_ppm(const framebuffer& fb, std::filesystem::path p);

} // namespace e3d::renderer