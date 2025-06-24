#version 330 core

in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

out vec4 fragColor;

uniform sampler2D day_texture;
uniform sampler2D normal_map;
uniform int useTexture;
uniform int useNormalMap;
uniform int useNightTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform mat3 normMatrix;

void main() {
    vec3 norm = normalize(normal);
    if (useNormalMap == 1) {
        vec3 normalMapColor = texture(normal_map, texCoord).rgb;
        normalMapColor = normalize(normalMapColor * 2.0 - 1.0);
        mat3 TBN = mat3(normalize(tangent), normalize(bitangent), normalize(normal));
        norm = normalize(TBN * normalMapColor);
    }

    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * vec3(1.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
    vec3 specular = 0.3 * spec * vec3(1.0);

    vec3 color = vec3(1.0, 0.0, 0.0);
    if (useTexture == 1) {
        color = texture(day_texture, texCoord).rgb;
    }

    float lightIntensity = max(dot(norm, lightDir), 0.0);
    float smoothIntensity = smoothstep(0.0, 0.5, lightIntensity);
    
    vec3 result = mix(ambient * color, (ambient + diffuse + specular) * color, smoothIntensity);
    fragColor = vec4(result, 1.0);
}
