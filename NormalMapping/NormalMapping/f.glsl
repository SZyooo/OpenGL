#version 330 core

in vec2 g_tCoord;
in vec3 g_pos;
in vec3 world_T;
in vec3 world_B;
in vec3 world_N;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

out vec4 color;

struct direct_light
{
    vec3 direction;
    float ambent;
    vec3 diffuse;
    vec3 specular;
    float shinness;
};
uniform direct_light dl;
uniform vec3 eye;

void main()
{
    mat3 TBN = mat3(world_T,world_B,world_N);
    vec3 frag2eye = eye - g_pos;
    //normal : norm in tangent space;
    vec3 norm_in_tangent = texture(normalMap,g_tCoord).rgb;
    norm_in_tangent = 2 * norm_in_tangent - 1;
    vec3 halfVec = normalize(normalize(-dl.direction) + normalize(frag2eye));
    frag2eye = normalize(TBN * frag2eye);
    halfVec = normalize(TBN * halfVec);
    float diff = max(dot(frag2eye,norm_in_tangent),0);
    float spec = pow(max(dot(halfVec,norm_in_tangent),0),dl.shinness * 2);
    vec3 original_color = texture(colorMap,g_tCoord).rgb;
    color = vec4((dl.ambent + diff * dl.diffuse + spec * dl.specular) * original_color,1);
}
