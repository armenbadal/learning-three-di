#version 460 core

out vec2 v_uv;

const vec2 positions[3] = vec2[](vec2(-1.0, -1.0), vec2( 3.0, -1.0), vec2(-1.0,  3.0));

void main()
{
    vec2 p = positions[gl_VertexID];
    gl_Position = vec4(p, 0.0, 1.0);
    vec2 uv = p * 0.5 + 0.5;
    v_uv = vec2(uv.x, 1.0 - uv.y);
}
