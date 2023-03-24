#version 430 core

uniform vec2 size;
out vec4 FragColor;

void main() {
  vec2 coord = gl_FragCoord.xy/size.xy;
  FragColor = vec4(coord.x, coord.y, 1.-coord.x, 1);
}