#version 430 core

out vec4 FragColor;
in vec2 myPosition;

uniform vec4 mySuperColor;

void main() {
  vec2 coord = gl_FragCoord.xy/myPosition.xy;
  FragColor = vec4(coord.x, coord.y, 1.-coord.x, 1);

  FragColor = mySuperColor;
}