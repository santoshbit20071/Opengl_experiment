#version 330 core

in vec3 out_color;
in vec2 out_texcod;

out vec4 color;

uniform sampler2D texture1;

void main() {
 color =  texture(texture1, out_texcod) ;
}
