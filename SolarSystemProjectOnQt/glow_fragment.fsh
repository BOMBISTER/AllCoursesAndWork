#version 330 core

in vec2 fragTexCoord;
out vec4 fragColor;

void main() {
    vec2 center = vec2(0.5, 0.5);
    float dist = length(fragTexCoord - center);
    float alpha = smoothstep(0.5, 0.0, dist);
    fragColor = vec4(1.0, 0.8, 0.4, alpha * 0.8);
}
