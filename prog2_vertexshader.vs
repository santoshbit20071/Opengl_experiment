#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_texcod;

out vec2 out_texcod;
out vec3 out_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection*view*model*vec4(position, 1.0f);
  out_color = in_color;
  out_texcod = in_texcod;
}
