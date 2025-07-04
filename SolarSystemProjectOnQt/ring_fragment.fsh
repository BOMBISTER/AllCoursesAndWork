#version 330 core

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D textureSampler;

void main() {
    vec4 texColor = texture(textureSampler, fragTexCoord);
    if (texColor.a < 0.1) discard;
    fragColor = texColor;
}
