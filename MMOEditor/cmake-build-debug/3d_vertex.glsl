#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 mvp;

out vec3 color;

void main()
{
    gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aPos;
    color.x *= 10;
    color.z *= 10;
}
