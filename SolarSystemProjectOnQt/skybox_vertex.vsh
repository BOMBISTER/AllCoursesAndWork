#version 330 core

in vec3 position;
out vec3 fragPos;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    fragPos = position;
    vec4 pos = projectionMatrix * viewMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
