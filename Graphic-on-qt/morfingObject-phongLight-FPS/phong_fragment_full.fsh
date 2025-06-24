#version 120

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int lightType;
uniform float lightIntensity;
uniform float lightAttenuation;
uniform vec3 lightDirection;
uniform float lightCutoff;
uniform float outerCutoff;

varying highp vec3 frag_pos;
varying highp vec3 norm_vec;
varying highp vec3 v_viewPos;
varying highp vec3 v_lightPos;

void main() {
    vec3 norm = normalize(norm_vec);


    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;


    vec3 lightDir;
    float attenuation = 1.0;
    if (lightType == 0) {
        lightDir = normalize(lightDirection);
    } else {
        lightDir = normalize(v_lightPos - frag_pos);
        float distance = length(v_lightPos - frag_pos);
        attenuation = lightIntensity / (1.0 + lightAttenuation * distance * distance);
    }


    float spotlightEffect = 1.0;
    if (lightType == 2) {
        float theta = dot(lightDir, normalize(lightDirection));
        float epsilon = lightCutoff - outerCutoff;
        spotlightEffect = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);
    }


    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    vec3 viewDir = normalize(v_viewPos - frag_pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = 0.5 * spec * lightColor;


    vec3 result = (ambient + (diffuse + specular) * attenuation * spotlightEffect) * objectColor;
    gl_FragColor = vec4(result, 1.0);
}
