cmake_minimum_required(VERSION 4.2)

# Enable experimental `import std;` support. The gate is a per-CMake-version
# UUID (here: CMake 4.4.0); see cmExperimental.cxx in CMake's sources.
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d" CACHE STRING "Enable experimental `import std` support")
set(CMAKE_CXX_MODULE_STD ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

# Homebrew's LLVM installs libc++.modules.json in a non-default directory that
# clang's driver cannot discover (LLVM issue #172497). Point CMake at it when it
# lives next to the compiler so that `import std;` works.
if(NOT CMAKE_CXX_STDLIB_MODULES_JSON AND CMAKE_CXX_COMPILER MATCHES "clang")
    get_filename_component(_llvm_prefix "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_llvm_prefix "${_llvm_prefix}" DIRECTORY)
    if(EXISTS "${_llvm_prefix}/lib/c++/libc++.modules.json")
        set(CMAKE_CXX_STDLIB_MODULES_JSON "${_llvm_prefix}/lib/c++/libc++.modules.json")
    endif()
endif()

project(learning-three-di)

enable_testing()

add_library(engine3d STATIC
    src/math/transform.cxx
    src/math/matrix4x4.cxx
    src/math/vector2.cxx
    src/math/vector3.cxx
    src/math/vector4.cxx
    src/renderer/framebuffer.cxx
    src/renderer/pipeline.cxx
    src/renderer/rasterizer.cxx
    src/renderer/clipping.cxx
    src/renderer/imager.cxx
    src/renderer/viewport.cxx
)
add_library(engine3d::engine3d ALIAS engine3d)

target_sources(engine3d
    PUBLIC
        FILE_SET CXX_MODULES FILES
            modules/e3d.math.cppm
            modules/e3d.graphics.cppm
            modules/e3d.geometry.cppm
            modules/e3d.camera.cppm
            modules/e3d.renderer.cppm
            modules/e3d.cppm
)

target_compile_features(engine3d PUBLIC cxx_std_23)

find_package(Catch2 3 REQUIRED)

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
