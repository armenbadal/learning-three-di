export module e3d.geometry;

export import e3d.math;
export import e3d.graphics;

export namespace e3d::geometry {

struct vertex2d {
    e3d::math::vector2 position;
    e3d::graphics::colour colour;
};

struct vertex3d {
    e3d::math::vector3 position;
    e3d::graphics::colour colour{e3d::graphics::white};
};

} // namespace e3d::geometry