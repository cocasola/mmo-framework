#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 mvp;

out vec3 color;

void main()
{
    gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = (aPos + vec3(1, 1, 1))/2.0;
}
