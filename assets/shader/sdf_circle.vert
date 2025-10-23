#version 460 core
in vec3 in_pos;
in vec4 in_circle;

out vec4 circle;

uniform mat4 projection;

void main() {
    circle = in_circle;
    gl_Position = projection * vec4(in_pos, 1.0);
}
