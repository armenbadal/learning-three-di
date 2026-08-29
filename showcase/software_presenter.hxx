#pragma once

#include <memory>

namespace e3d::renderer {
class framebuffer;
} // namespace e3d::renderer

namespace showcase {

class software_presenter final {
public:
    software_presenter();
    ~software_presenter();

    software_presenter(const software_presenter&) = delete;
    software_presenter& operator=(const software_presenter&) = delete;

    software_presenter(software_presenter&&) = delete;
    software_presenter& operator=(software_presenter&&) = delete;

    void present(const e3d::renderer::framebuffer& framebuffer);

private:
    class implementation;
    std::unique_ptr<implementation> _impl;
};

} // namespace showcase
