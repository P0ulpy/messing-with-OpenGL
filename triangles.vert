#version 430 core

layout (location = 0) in vec4 vPosition;

varying vec2 myPosition;

void main()
{
    gl_Position = vPosition;
    myPosition = vPosition.xy;
}