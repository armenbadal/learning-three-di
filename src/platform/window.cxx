#include "engine3d/platform/window.hxx"

namespace e3d::platform {

window::extent window::size() const noexcept
{
    return {};
}

[[nodiscard]]
bool window::should_close() const noexcept
{
    return false;
}

void window::poll_events()
{}

void window::swap_buffers()
{}

} // namespace e3d::platform
