#version 330 core

in vec3 out_color;
in vec2 tex_coord;

out vec4 color;
uniform sampler2D tex;

void main() {
    //color = texture(tex, tex_coord);
    color = vec4(out_color, 1.0);
}
