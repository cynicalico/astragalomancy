#version 460 core

in vec4 circle;

out vec4 FragColor;

float sdf_circle(vec2 p, float r) {
    return length(p) - r;
}

float stroke_sdf_circle(vec2 p, float r, float t) {
    return abs(sdf_circle(p, r)) - (t / 2.0);
}

void main() {
    vec2 sdf_p = vec2(gl_FragCoord.x - circle.x, gl_FragCoord.y - circle.y);

    float distance;
    if (circle.w == 0.0) {
        distance = sdf_circle(sdf_p, circle.z);
    } else {
        distance = stroke_sdf_circle(sdf_p, circle.z, circle.w);
    }
    if (distance > 0) {
        discard;
    }

    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
