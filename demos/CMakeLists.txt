function(add_engine3d_demo target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} PRIVATE engine3d::engine3d)
endfunction()

add_engine3d_demo(framebuffer-demo framebuffer_demo.cxx)
add_engine3d_demo(rasterizer-demo rasterizer_demo.cxx)
add_engine3d_demo(pipeline-demo pipeline_demo.cxx)
add_engine3d_demo(cube-demo cube_demo.cxx)
