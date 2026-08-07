add_library(math-3d STATIC)

target_sources(math-3d PRIVATE src/vector2.cxx)
target_include_directories(math-3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(math-3d-tests)
target_sources(math-3d-tests PRIVATE src/vector2.cxx tests/vector2_test.cxx)
target_include_directories(math-3d-tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
