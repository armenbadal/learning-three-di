cmake_minimum_required(VERSION 4.2)

include(cmake/Engine3DToolchain.cmake)

project(learning-three-di)

enable_testing()

set(engine3d_sources
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

set(engine3d_modules
    modules/e3d.math.cppm
    modules/e3d.graphics.cppm
    modules/e3d.geometry.cppm
    modules/e3d.camera.cppm
    modules/e3d.renderer.cppm
    modules/e3d.cppm
)

set(engine3d_test_sources
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

add_library(engine3d STATIC ${engine3d_sources})
add_library(engine3d::engine3d ALIAS engine3d)

target_sources(engine3d
    PUBLIC
        FILE_SET CXX_MODULES FILES ${engine3d_modules}
)

target_compile_features(engine3d PUBLIC cxx_std_23)

find_package(Catch2 3 REQUIRED)

add_executable(engine3d-tests ${engine3d_test_sources})
target_link_libraries(engine3d-tests PRIVATE engine3d::engine3d Catch2::Catch2WithMain)

add_test(NAME engine3d-tests COMMAND engine3d-tests)

add_subdirectory(demos)
