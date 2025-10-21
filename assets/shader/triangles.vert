#version 460 core

in vec3 in_pos;
in vec3 in_color;

out vec3 color;

uniform mat4 projection;

void main() {
    color = in_color;
    gl_Position = projection * vec4(in_pos, 1.0);
}
