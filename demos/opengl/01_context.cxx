
// 1. ստեղծել demos/opengl/01_context.cxx,
// 2. ավելացնել GLFW + GLAD dependency-ները,
// 3. հաջող compile անել,
// 4. ստանալ 800×600 մուգ window,
// 5. console-ում տպել իրական OpenGL version-ը,
// 6. window-ի close button-ը սեղմելիս ծրագիրը ճիշտ ավարտել։

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

    glfwMakeContextCurrent(window);

    if( gladLoadGL(glfwGetProcAddress) == 0 ) {
        std::cerr << "Failed to load OpenGL functions.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }

    std::cout << "OpenGL: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << '\n';

    while( !glfwWindowShouldClose(window) ) {
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
