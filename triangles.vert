#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 textCoord;
//layout (location = 1) in vec4 color;

uniform mat4 model;

//out vec4 someColor;
out vec2 fragTextCoord;

void main()
{
    gl_Position = model * vPosition;
    //someColor = color;
    fragTextCoord = textCoord.xy;
}