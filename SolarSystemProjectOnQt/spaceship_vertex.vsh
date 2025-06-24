#version 330 core

layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 normalAttr;
layout(location = 2) in vec2 texAttr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    FragPos = vec3(model * vec4(posAttr, 1.0));
    Normal = mat3(transpose(inverse(model))) * normalAttr;
    TexCoord = texAttr;
    gl_Position = projection * view * model * vec4(posAttr, 1.0);
}
