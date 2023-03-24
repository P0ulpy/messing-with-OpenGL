#version 430 core

uniform vec2 size;
out vec4 FragColor;
varying vec2 myPosition;

void main() {
  vec2 coord = gl_FragCoord.xy/size.xy;
  FragColor = vec4(myPosition.x, myPosition.y, 1.-myPosition.x, 1);
}