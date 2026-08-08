add_executable(framebuffer-demo
    framebuffer_demo.cxx
)
target_link_libraries(framebuffer-demo PRIVATE renderer)