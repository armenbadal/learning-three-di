add_executable(framebuffer-demo framebuffer_demo.cxx)
target_link_libraries(framebuffer-demo PRIVATE engine3d::engine3d)

add_executable(rasterizer-demo rasterizer_demo.cxx)
target_link_libraries(rasterizer-demo PRIVATE engine3d::engine3d)
