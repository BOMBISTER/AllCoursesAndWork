#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in mat3 TBN;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform bool useNormalMap;
uniform sampler2D textureSampler;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Sample texture
    vec4 texColor = texture(textureSampler, TexCoord);

    // Combine results
    vec3 result = (ambient + diffuse + specular) * objectColor * texColor.rgb;
    FragColor = vec4(result, texColor.a);
}
