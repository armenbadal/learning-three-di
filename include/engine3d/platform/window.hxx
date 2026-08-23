#pragma once

#include <cstdint>

namespace e3d::platform {

class window {
public:
    struct extent {
        std::uint32_t width;
        std::uint32_t height;
    };

    [[nodiscard]]
    extent size() const noexcept;

    [[nodiscard]]
    bool should_close() const noexcept;

    void poll_events();
    void swap_buffers();
};

} // namespace e3d::platform
