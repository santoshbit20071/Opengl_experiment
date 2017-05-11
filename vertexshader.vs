#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texturecoord;

out vec3 out_color;
out vec2 tex_coord;

void main() {
  gl_Position = vec4(position, 0.0);
  out_color = color;
  tex_coord = texturecoord;
}
