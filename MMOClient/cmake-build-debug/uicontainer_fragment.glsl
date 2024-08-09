#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D tex;
uniform int use_texture;

void main()
{
    if (use_texture == 1) {
        FragColor = texture(tex, uv);
    } else {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
}
