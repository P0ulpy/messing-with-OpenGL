#version 430 core

layout (location = 0) in vec4 vPosition;
//layout (location = 1) in vec4 color;
layout (location = 1) in vec4 textCoord;

//out vec4 someColor;
out vec2 fragTextCoord;

void main()
{
    gl_Position = vPosition;
    //someColor = color;
    fragTextCoord = textCoord.xy;
}