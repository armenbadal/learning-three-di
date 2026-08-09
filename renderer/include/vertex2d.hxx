#pragma once

#include "colour.hxx"
#include "vector2.hxx"

namespace renderer {

struct vertex2d {
    math::vector2 _position;
    colour _colour;
};

} // namespace renderer
