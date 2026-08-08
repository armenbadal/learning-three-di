add_executable(framebuffer-demo framebuffer_demo.cxx)
target_link_libraries(framebuffer-demo PRIVATE renderer)

add_executable(rasterizer-demo rasterizer_demo.cxx)
target_link_libraries(rasterizer-demo PRIVATE renderer math-3d)
