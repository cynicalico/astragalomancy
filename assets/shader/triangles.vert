#version 460 core

in vec3 in_pos;
in vec3 in_color;

out vec3 color;

#include "assets/shader/common.shader"

void main() {
    color = in_color;
    gl_Position = projection * vec4(in_pos, 1.0);
}
