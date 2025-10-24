#version 460 core
in vec4 circle;

out vec4 FragColor;

float sdf_circle(vec2 p, float r) {
    return length(p) - r;
}

float stroke_sdf_circle(vec2 p, float r, float t) {
    if (t == 1.0) {
        // For pixel-perfect 1px stroke: check if circle boundary crosses this pixel
        vec2 pixel_min = floor(p);
        vec2 pixel_max = pixel_min + vec2(1.0);
        vec2 pixel_center = pixel_min + vec2(0.5);

        // Find min and max distances within this pixel
        float min_d = length(pixel_min);
        float max_d = min_d;

        // Check all 4 corners
        float d;
        d = length(vec2(pixel_max.x, pixel_min.y));
        min_d = min(min_d, d);
        max_d = max(max_d, d);

        d = length(vec2(pixel_min.x, pixel_max.y));
        min_d = min(min_d, d);
        max_d = max(max_d, d);

        d = length(pixel_max);
        min_d = min(min_d, d);
        max_d = max(max_d, d);

        // Also check center
        float center_d = length(pixel_center);
        min_d = min(min_d, center_d);
        max_d = max(max_d, center_d);

        // Base check: does circle boundary cross this pixel?
        bool crosses = (min_d < r && r < max_d);

        // For small radii only, add corner-cutting logic
        if (crosses && r <= 5.0) {
            // Reject pixels whose center is too far from the radius
            if (abs(center_d - r) > 0.5) {
                return 0.5;
            }
        }

        if (crosses) {
            return -0.5;
        } else {
            return 0.5;
        }
    }
    return abs(sdf_circle(p, r)) - (t / 2.0);
}

void main() {
    vec2 center = vec2(circle.x, circle.y);
    vec2 frag_pos = gl_FragCoord.xy;
    vec2 sdf_p = frag_pos - center;

    float distance;
    if (circle.w == 0.0) {
        distance = sdf_circle(sdf_p, circle.z);
    } else {
        distance = stroke_sdf_circle(sdf_p, circle.z, circle.w);
    }
    if (distance > 0) {
        discard;
    }

    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
