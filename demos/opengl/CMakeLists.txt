add_executable(opengl-context-demo 01_context.cxx)
target_link_libraries(opengl-context-demo PRIVATE engine3d::engine3d glfw glad_gl_core_46 OpenGL::GL)
target_include_directories(opengl-context-demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(opengl-triangle-demo 02_triangle.cxx)
target_link_libraries(opengl-triangle-demo PRIVATE engine3d::engine3d glfw glad_gl_core_46 OpenGL::GL)
target_include_directories(opengl-triangle-demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(opengl-vertex-colors-demo 03_vertex_colors.cxx)
target_link_libraries(opengl-vertex-colors-demo PRIVATE engine3d::engine3d glfw glad_gl_core_46 OpenGL::GL)
target_include_directories(opengl-vertex-colors-demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(opengl-indexed-quad-demo 04_indexed_quad.cxx)
target_link_libraries(opengl-indexed-quad-demo PRIVATE engine3d::engine3d glfw glad_gl_core_46 OpenGL::GL)
target_include_directories(opengl-indexed-quad-demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(opengl-uniform-model-transform-demo 05_uniform_model_transform.cxx)
target_link_libraries(opengl-uniform-model-transform-demo PRIVATE engine3d::engine3d glfw glad_gl_core_46 OpenGL::GL)
target_include_directories(opengl-uniform-model-transform-demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


function(copy_shaders target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/shaders"
            "$<TARGET_FILE_DIR:${target}>"
        COMMENT "Copying shaders for ${target}"
        VERBATIM
    )
endfunction()

copy_shaders(opengl-context-demo)
copy_shaders(opengl-triangle-demo)
copy_shaders(opengl-vertex-colors-demo)
copy_shaders(opengl-uniform-model-transform-demo)
