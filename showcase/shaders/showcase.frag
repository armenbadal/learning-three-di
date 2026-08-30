#version 460 core

in vec2 v_uv;

uniform sampler2D u_framebuffer;

out vec4 out_colour;

void main()
{
    out_colour = texture(u_framebuffer, v_uv);
}
