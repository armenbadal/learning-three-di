export module e3d.graphics;

import std;

export namespace e3d::graphics {

class colour {
public:
    constexpr colour(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) noexcept
        : _r{r}, _g{g}, _b{b}, _a{a}
    {}

    constexpr std::uint8_t r() const noexcept { return _r; }
    constexpr std::uint8_t g() const noexcept { return _g; }
    constexpr std::uint8_t b() const noexcept { return _b; }
    constexpr std::uint8_t a() const noexcept { return _a; }

    constexpr bool operator==(const colour&) const noexcept = default;

    constexpr colour operator+(const colour& other) const noexcept
    {
        const auto sum = [](std::uint8_t a, std::uint8_t b) {
            const auto s = static_cast<unsigned int>(a) + static_cast<unsigned int>(b);
            return static_cast<std::uint8_t>(s < 255U ? s : 255U);
        };
        return {sum(_r, other._r), sum(_g, other._g), sum(_b, other._b), sum(_a, other._a)};
    }

    constexpr colour operator*(float scalar) const noexcept
    {
        const auto scale = [scalar](std::uint8_t c) {
            const float v = static_cast<float>(c) * scalar;
            const float clamped = v < 0.0F ? 0.0F : (v > 255.0F ? 255.0F : v);
            return static_cast<std::uint8_t>(clamped + 0.5F);
        };
        return {scale(_r), scale(_g), scale(_b), scale(_a)};
    }

private:
    std::uint8_t _r;
    std::uint8_t _g;
    std::uint8_t _b;
    std::uint8_t _a;
};

class colourf {
public:
    constexpr colourf() noexcept = default;
    constexpr colourf(float r, float g, float b, float a = 255.0F) noexcept
        : _r{r}, _g{g}, _b{b}, _a{a}
    {}

    constexpr float r() const noexcept { return _r; }
    constexpr float g() const noexcept { return _g; }
    constexpr float b() const noexcept { return _b; }
    constexpr float a() const noexcept { return _a; }

    constexpr bool operator==(const colourf&) const noexcept = default;

private:
    float _r{255.0F};
    float _g{255.0F};
    float _b{255.0F};
    float _a{255.0F};
};

constexpr colourf operator+(const colourf& lhs, const colourf& rhs) noexcept
{
    return {lhs.r() + rhs.r(), lhs.g() + rhs.g(), lhs.b() + rhs.b(), lhs.a() + rhs.a()};
}

constexpr colourf operator-(const colourf& lhs, const colourf& rhs) noexcept
{
    return {lhs.r() - rhs.r(), lhs.g() - rhs.g(), lhs.b() - rhs.b(), lhs.a() - rhs.a()};
}

constexpr colourf operator*(const colourf& c, float scalar) noexcept
{
    return {c.r() * scalar, c.g() * scalar, c.b() * scalar, c.a() * scalar};
}

constexpr colourf operator/(const colourf& c, float scalar) noexcept
{
    return {c.r() / scalar, c.g() / scalar, c.b() / scalar, c.a() / scalar};
}

constexpr colourf to_colourf(const colour& c) noexcept
{
    return {
        static_cast<float>(c.r()),
        static_cast<float>(c.g()),
        static_cast<float>(c.b()),
        static_cast<float>(c.a())
    };
}

constexpr colour to_colour(const colourf& c) noexcept
{
    const auto channel = [](float value) {
        const float clamped = value < 0.0F ? 0.0F : (value > 255.0F ? 255.0F : value);
        return static_cast<std::uint8_t>(clamped + 0.5F);
    };

    return {channel(c.r()), channel(c.g()), channel(c.b()), channel(c.a())};
}

inline constexpr colour white{255, 255, 255};
inline constexpr colour black{0, 0, 0};

} // namespace e3d::graphics