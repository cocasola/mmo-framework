#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D tex;
uniform int use_texture;
uniform vec4 colour;

void main()
{
    if (use_texture == 1) {
        FragColor = vec4(1.0, 1.0, 1.0, texture(tex, uv).r)*colour;
    } else {
        FragColor = colour;
    }
}
