#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 model;

out vec4 iColor;

void main()
{
    gl_Position = model * vPosition;
    iColor = vColor;
}