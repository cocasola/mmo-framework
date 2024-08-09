#version 330 core

out uint value;

uniform uint callbacks;

void main()
{
    value = callbacks;
}
