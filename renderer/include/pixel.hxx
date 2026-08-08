#pragma once

#include <cstdint>

namespace renderer {

class pixel {
public:
    constexpr pixel(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255)
        : _r{r}, _g{g}, _b{b}, _a{a}
    {}

    constexpr std::uint8_t r() const { return _r; }
    constexpr std::uint8_t g() const { return _g; }
    constexpr std::uint8_t b() const { return _b; }
    constexpr std::uint8_t a() const { return _a; }

private:
    std::uint8_t _r;
    std::uint8_t _g;
    std::uint8_t _b;
    std::uint8_t _a;
};

constexpr pixel white{255, 255, 255};
constexpr pixel black{0, 0, 0};

} // namespace renderer
