
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

GLuint compile_shader(GLenum type, std::filesystem::path f)
{
    std::string source;
    std::ifstream fin{f};
    if( !fin ) {
        std::cerr << "Failed to open shader: " << f << '\n';
        return 0;
    }
    source.resize(std::filesystem::file_size(f));
    fin.read(source.data(), source.size());

    const GLuint shader = glCreateShader(type);
    const GLchar* sp = source.c_str();
    glShaderSource(shader, 1, &sp, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if( success == GL_FALSE ) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string log(static_cast<std::size_t>(length), '\0');
        glGetShaderInfoLog(shader, length, nullptr, log.data());
        std::cerr << log << '\n';
    }

    return shader;
}

GLuint create_program(GLuint vertex_shader, GLuint fragment_shader)
{
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if( success == GL_FALSE ) {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::string log(static_cast<std::size_t>(length), '\0');
        glGetProgramInfoLog(program, length, nullptr, log.data());
        std::cerr << log << '\n';
    }

    return program;
}

void framebuffer_size_callback(GLFWwindow* wnd, int w, int h)
{
    glViewport(0, 0, w, h);
}

struct vertex {
    float position[3];
    float colour[3];
};

int main(int argc, char* argv[])
{
    if( !glfwInit() ) {
        std::cerr << "Failed to initialize GLFW.\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow(800, 600, "OpenGL, Context demo", nullptr, nullptr);
    if( window == nullptr ) {
        std::cerr << "Failed to create window.\n";
        glfwTerminate();
        return 2;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    if( gladLoadGL(glfwGetProcAddress) == 0 ) {
        std::cerr << "Failed to load OpenGL functions.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }

    std::cout << "OpenGL: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << '\n';

    const auto shader_directory = std::filesystem::absolute(argv[0]).parent_path();
    auto vertex_shader = compile_shader(GL_VERTEX_SHADER, shader_directory / "vertex_color.vert");
    auto fragment_shader = compile_shader(GL_FRAGMENT_SHADER, shader_directory / "vertex_color.frag");
    if( vertex_shader == 0 || fragment_shader == 0 ) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 4;
    }
    auto shader_program = create_program(vertex_shader, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Quad demo
    constexpr vertex vertices[] = {
        // position             // color
        {{-0.5F, -0.5F, 0.0F}, {1.0F, 0.0F, 0.0F}}, // 0
        {{ 0.5F, -0.5F, 0.0F}, {0.0F, 1.0F, 0.0F}}, // 1
        {{ 0.5F,  0.5F, 0.0F}, {0.0F, 0.0F, 1.0F}}, // 2
        {{-0.5F,  0.5F, 0.0F}, {1.0F, 1.0F, 0.0F}}  // 3
    };

    constexpr unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, colour)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    while( !glfwWindowShouldClose(window) ) {
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shader_program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
