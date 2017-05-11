#version 330 core

in vec3 out_color;
in vec2 texCoordinate;

uniform sampler2D tex;

out vec4 color;

void main() {
    color = texture(tex, texCoordinate);
}
