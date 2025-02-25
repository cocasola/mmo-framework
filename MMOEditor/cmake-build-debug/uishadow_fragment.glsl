#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D tex;
uniform int horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 tex_offset = vec2(1.0, 1.0) / textureSize(tex, 0); // gets size of single texel
    vec4 result = texture(tex, uv) * weight[0]; // current fragment's contribution
    if(horizontal == 1)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(tex, uv + vec2(tex_offset.x * i, 0.0)) * weight[i];
            result += texture(tex, uv - vec2(tex_offset.x * i, 0.0)) * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(tex, uv + vec2(0.0, tex_offset.y * i)) * weight[i];
            result += texture(tex, uv - vec2(0.0, tex_offset.y * i)) * weight[i];
        }
    }
    FragColor = result;
}
