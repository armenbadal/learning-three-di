#pragma once

#include <cstdint>
#include <memory>
#include <string_view>

namespace e3d::platform {

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

    void poll_events();
    void swap_buffers();

private:
    class implementation;
    std::unique_ptr<implementation> _impl;
};

} // namespace e3d::platform
