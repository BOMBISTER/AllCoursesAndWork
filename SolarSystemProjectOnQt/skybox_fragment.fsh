#version 330 core

in vec3 fragPos;
out vec4 fragColor;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    vec3 dir = normalize(fragPos);
    vec2 uv = vec2(atan(dir.z, dir.x) / (2.0 * 3.14159), acos(dir.y) / 3.14159);

    float star = random(uv * 1000.0);
    vec3 color = vec3(0.0);
    if (star > 0.995) {
        color = vec3(0.3);
    } else if (star > 0.993) {
        color = vec3(0.2, 0.2, 0.3);
    }

    fragColor = vec4(color, 1.0);
}
