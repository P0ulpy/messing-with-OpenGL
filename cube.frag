#version 430 core

out vec4 fragColor;

in vec4 iColor;

void main()
{
    fragColor = iColor;
}