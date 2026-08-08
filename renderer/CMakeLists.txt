add_library(renderer STATIC
    src/renderer.cxx
    src/framebuffer.cxx
    src/utility.cxx
)
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(renderer PUBLIC math-3d)

add_executable(renderer-tests
    tests/pixel_test.cxx
    tests/framebuffer_test.cxx
    tests/utility_test.cxx
)
find_package(Catch2 3 REQUIRED)
target_link_libraries(renderer-tests PRIVATE renderer Catch2::Catch2WithMain)

add_test(NAME renderer-tests COMMAND renderer-tests)