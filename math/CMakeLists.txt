add_library(math-3d STATIC
    src/vector2.cxx
    src/vector3.cxx
    src/vector4.cxx
)
target_include_directories(math-3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(math-3d-tests
    tests/math_test.cxx
    tests/vector2_test.cxx
    tests/vector3_test.cxx
    tests/vector4_test.cxx
)
target_link_libraries(math-3d-tests PRIVATE math-3d)

add_test(NAME math-3d-tests COMMAND math-3d-tests)
