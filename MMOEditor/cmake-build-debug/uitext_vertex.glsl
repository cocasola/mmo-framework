#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 2) in vec2 v_uv;

uniform ivec2 size;
uniform ivec2 pos;
uniform float depth;
uniform float max_depth;
uniform ivec2 window_size;
uniform int pixel_scale;

out vec2 uv;

void main()
{
    float x = v_pos.x;
    float y = v_pos.y;

    x *= size.x;
    y *= size.y;

    x += pos.x;
    y -= pos.y;

    x *= pixel_scale;
    y *= pixel_scale;

    x /= window_size.x/2;
    y /= window_size.y/2;

    x -= 1.0f;
    y += 1.0f;

    float z = (max_depth - depth)/max_depth*2.0 - 1.0;

    gl_Position = vec4(x, y, z, 1.0);
    uv = v_uv;
}
