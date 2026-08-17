add_executable(first-triangle first_triangle.cxx)

target_link_libraries(first-triangle PRIVATE engine3d::engine3d glfw glad::glad OpenGL::GL)

target_include_directories(first-triangle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
