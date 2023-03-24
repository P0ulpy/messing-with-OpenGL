#version 430 core

out vec4 FragColor;
//in vec4 someColor;

in vec2 fragTextCoord;

uniform sampler2D texture1;

void main() {
  //vec2 coord = gl_FragCoord.xy/myPosition.xy;
  //FragColor = vec4(coord.x, coord.y, 1.-coord.x, 1);

  //FragColor = someColor;

    FragColor = texture(texture1, fragTextCoord);
}