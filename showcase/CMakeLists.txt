add_executable(showcase
    main.cxx
    application.cxx
    software_presenter.cxx
)

target_link_libraries(showcase PRIVATE
    engine3d::engine3d
    glfw
    glad_gl_core_46
    OpenGL::GL
)

add_custom_command(TARGET showcase POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:showcase>/shaders"
    COMMENT "Copying shaders for showcase"
    VERBATIM
)
