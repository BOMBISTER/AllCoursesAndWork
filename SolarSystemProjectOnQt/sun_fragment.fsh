#version 330 core

in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform bool useTexture;
uniform bool useNormalMap;
uniform bool useNightTexture;
uniform sampler2D textureSampler;

void main() {
    // Ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine
    vec3 result = (ambient + diffuse + specular) * objectColor;

    // Add texture
    if (useTexture) {
        vec4 texColor = texture(textureSampler, texCoord);
        result = result * texColor.rgb;
    }

    // Add emission for sun
    vec3 emission = vec3(1.0, 0.8, 0.2) * 0.5; // Теплый желтый цвет свечения
    result += emission;

    fragColor = vec4(result, 1.0);
} 
