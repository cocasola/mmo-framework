#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D tex;
uniform int use_texture;
uniform vec4 colour;
uniform ivec2 size;
uniform float radius;

void main()
{
    vec2 coords = uv * size;

    if (length(coords - vec2(0)) < radius) {
        if (length(coords - vec2(radius, radius)) > radius)
            discard;
    }

    if (length(coords - vec2(0, size.y)) < radius) {
        if (length(coords - vec2(radius, size.y - radius)) > radius)
            discard;
    }

    if (length(coords - vec2(size.x, 0)) < radius) {
        if (length(coords - vec2(size.x - radius, radius)) > radius)
            discard;
    }

    if (length(coords - size) < radius) {
        if (length(coords - vec2(size.x - radius, size.y - radius)) > radius)
            discard;
    }

    if (use_texture == 1) {
        FragColor = texture(tex, uv);
    } else {
        FragColor = colour;
    }
}
