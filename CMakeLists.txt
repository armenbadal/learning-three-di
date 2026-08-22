cmake_minimum_required(VERSION 4.2)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

project(learning-three-di)

enable_testing()

include(FetchContent)

FetchContent_Declare(
    glad2
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.6
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR cmake
)
FetchContent_MakeAvailable(glad2)

# Generate GLAD 2 for the OpenGL core profile used by the GLFW demo.
glad_add_library(glad_gl_core_46 STATIC REPRODUCIBLE LOADER API gl:core=4.6)

add_library(engine3d STATIC
    src/camera/camera.cxx
    src/renderer/clipping.cxx
    src/math/transform.cxx
    src/math/matrix4x4.cxx
    src/math/vector2.cxx
    src/math/vector3.cxx
    src/math/vector4.cxx
    src/renderer/framebuffer.cxx
    src/renderer/pipeline.cxx
    src/renderer/rasterizer.cxx
    src/renderer/imager.cxx
    src/renderer/viewport.cxx
    src/renderer/software_renderer.cxx
    src/renderer/opengl/opengl_renderer.cxx
    src/renderer/opengl/shader.cxx
    src/renderer/opengl/shader_program.cxx
    src/renderer/opengl/buffer.cxx
    src/renderer/opengl/vertex_array.cxx
    src/renderer/opengl/gpu_mesh.cxx
    src/platform/window.cxx
)
add_library(engine3d::engine3d ALIAS engine3d)

target_compile_features(engine3d PUBLIC cxx_std_23)
target_include_directories(engine3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Catch2 3 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(engine3d-tests
    tests/camera/camera_test.cxx
    tests/renderer/clipping_test.cxx
    tests/geometry/vertex_test.cxx
    tests/math/transform_test.cxx
    tests/math/matrix4x4_test.cxx
    tests/math/vector2_test.cxx
    tests/math/vector3_test.cxx
    tests/math/vector4_test.cxx
    tests/graphics/colour_test.cxx
    tests/renderer/framebuffer_test.cxx
    tests/renderer/pipeline_integration_test.cxx
    tests/renderer/pipeline_test.cxx
    tests/renderer/rasterizer_test.cxx
    tests/renderer/imager_test.cxx
    tests/renderer/viewport_test.cxx
)
target_link_libraries(engine3d-tests PRIVATE engine3d::engine3d Catch2::Catch2WithMain)

add_test(NAME engine3d-tests COMMAND engine3d-tests)

add_subdirectory(demos)
