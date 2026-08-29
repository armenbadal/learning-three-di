#include "engine3d/platform/window.hxx"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <array>
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

    bool key_down(key value) const noexcept
    {
        return _current_keys[to_index(value)];
    }

    bool key_pressed(key value) const noexcept
    {
        const auto index = to_index(value);
        return _current_keys[index] && !_previous_keys[index];
    }

    void request_close() noexcept
    {
        glfwSetWindowShouldClose(_handle, GLFW_TRUE);
    }

    void poll_events()
    {
        _previous_keys = _current_keys;
        glfwPollEvents();
        update_key_states();
    }

    void swap_buffers()
    {
        glfwSwapBuffers(_handle);
    }

private:
    static constexpr std::size_t key_count = static_cast<std::size_t>(key::count);

    static constexpr std::size_t to_index(key value) noexcept
    {
        return static_cast<std::size_t>(value);
    }

    static constexpr std::array native_keys{
        GLFW_KEY_ESCAPE,
        GLFW_KEY_W,
        GLFW_KEY_A,
        GLFW_KEY_S,
        GLFW_KEY_D,
        GLFW_KEY_Z,
        GLFW_KEY_C,
        GLFW_KEY_1,
        GLFW_KEY_2,
    };

    void update_key_states() noexcept
    {
        for( std::size_t index = 0; index < key_count; ++index )
            _current_keys[index] = glfwGetKey(_handle, native_keys[index]) != GLFW_RELEASE;
    }

private:
    std::string _title;

    GLFWwindow* _handle{nullptr};
    std::array<bool, key_count> _previous_keys{};
    std::array<bool, key_count> _current_keys{};
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

bool window::key_down(key value) const noexcept
{
    return _impl->key_down(value);
}

bool window::key_pressed(key value) const noexcept
{
    return _impl->key_pressed(value);
}

void window::request_close() noexcept
{
    _impl->request_close();
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
