cmake_minimum_required(VERSION 4.2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

project(learning-three-di)

enable_testing()

add_library(engine3d STATIC
    src/camera/camera.cxx
    src/geometry/polygon.cxx
    src/math/transform.cxx
    src/math/matrix4x4.cxx
    src/math/vector2.cxx
    src/math/vector3.cxx
    src/math/vector4.cxx
    src/renderer/framebuffer.cxx
    src/renderer/rasterizer.cxx
    src/renderer/imager.cxx
)
add_library(engine3d::engine3d ALIAS engine3d)

target_compile_features(engine3d PUBLIC cxx_std_23)
target_include_directories(engine3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Catch2 3 REQUIRED)

add_executable(engine3d-tests
    tests/camera/camera_test.cxx
    tests/geometry/polygon_test.cxx
    tests/geometry/vertex_test.cxx
    tests/math/transform_test.cxx
    tests/math/matrix4x4_test.cxx
    tests/math/vector2_test.cxx
    tests/math/vector3_test.cxx
    tests/math/vector4_test.cxx
    tests/renderer/colour_test.cxx
    tests/renderer/framebuffer_test.cxx
    tests/renderer/rasterizer_test.cxx
    tests/renderer/imager_test.cxx
)
target_link_libraries(engine3d-tests PRIVATE engine3d::engine3d Catch2::Catch2WithMain)

add_test(NAME engine3d-tests COMMAND engine3d-tests)

add_subdirectory(demos)
