#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string_view>

namespace e3d::platform {

enum class key {
    escape,
    w,
    a,
    s,
    d,
    z,
    c,
    one,
    two,
    count,
};

enum class key_state
{
    released,
    pressed
};

class window {
public:
    struct extent {
        std::uint32_t width;
        std::uint32_t height;
    };

    window(extent size, std::string_view title);

    window(const window&) = delete;
    window& operator=(const window&) = delete;
    window(window&&) = delete;
    window& operator=(window&&) = delete;

    ~window();

    [[nodiscard]]
    extent size() const noexcept;

    [[nodiscard]]
    bool should_close() const noexcept;

    [[nodiscard]]
    bool key_down(key value) const noexcept;

    [[nodiscard]]
    bool key_pressed(key value) const noexcept;

    void request_close() noexcept;
    void poll_events();
    void swap_buffers();

private:
    class implementation;
    std::unique_ptr<implementation> _impl;
};

} // namespace e3d::platform
