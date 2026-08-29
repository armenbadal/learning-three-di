#include "engine3d/platform/window.hxx"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <string>

namespace e3d::platform {

class window::implementation {
public:
    implementation(extent size, std::string_view title)
        : _title{title}
    {
        if( !glfwInit() )
            throw std::runtime_error{"Cannot initialize graphics system."};

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _handle = glfwCreateWindow(size.width, size.height, _title.c_str(), nullptr, nullptr);
        if( _handle == nullptr ) {
            glfwTerminate();
            throw std::runtime_error{"Cannot create window."};
        }

        glfwMakeContextCurrent(_handle);

        if( gladLoadGL(glfwGetProcAddress) == 0 ) {
            glfwDestroyWindow(_handle);
            glfwTerminate();
            throw std::runtime_error{"Failed to load OpenGL functions."};
        }
    }

    ~implementation()
    {
        glfwDestroyWindow(_handle);
        glfwTerminate();
    }

    extent size() const noexcept
    {
        int width{0}, height{0};
        glfwGetFramebufferSize(_handle, &width, &height);
        return {static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height)};
    }

    bool should_close() const noexcept
    {
        return glfwWindowShouldClose(_handle);
    }

    void poll_events()
    {
        glfwPollEvents();
    }

    void swap_buffers()
    {
        glfwSwapBuffers(_handle);
    }

    std::string _title;

    GLFWwindow* _handle{nullptr};
};

window::window(extent size, std::string_view title)
    : _impl{std::make_unique<implementation>(size, title)}
{}

window::~window() = default;

window::extent window::size() const noexcept
{
    return _impl->size();
}

bool window::should_close() const noexcept
{
    return _impl->should_close();
}

void window::poll_events()
{
    _impl->poll_events();
}

void window::swap_buffers()
{
    _impl->swap_buffers();
}

} // namespace e3d::platform
