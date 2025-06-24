#version 120

attribute highp vec4 posAttr;
attribute highp vec3 normAttr;

uniform highp mat4 projection_matrix;
uniform highp mat3 norm_matrix;
uniform highp mat4 model;
uniform highp mat4 view_matrix;
uniform highp float morph_param;
uniform highp vec3 lightPos;
uniform highp vec3 viewPos;

varying highp vec3 v_lightPos;
varying highp vec3 norm_vec;
varying highp vec3 frag_pos;
varying highp vec3 v_viewPos;

void main() {

    float x = posAttr.x;
    float y = posAttr.y;
    float z = posAttr.z;
    float rad = sqrt(x*x + y*y + z*z);
    float phi = atan(y, x);
    float theta = acos(z / rad);
    float norm_rad = 1.0;
    float morph_rad = morph_param * rad + (1.0 - morph_param) * norm_rad;
    vec4 morph_posit = vec4(morph_rad * sin(theta) * cos(phi),
                            morph_rad * sin(theta) * sin(phi),
                            morph_rad * cos(theta), 1.0);

    vec4 world_pos = model * morph_posit;

    gl_Position = projection_matrix * view_matrix * world_pos;

    frag_pos = vec3(world_pos);

    vec3 spherical_normal = normalize(vec3(morph_posit.x, morph_posit.y, morph_posit.z));
    norm_vec = normalize(norm_matrix * mix(normAttr, spherical_normal, 1.0 - morph_param));

    v_viewPos = viewPos;
    v_lightPos = lightPos;
}
