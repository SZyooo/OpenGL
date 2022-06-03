#version 330 core

in vec2 tCoord;
in vec4 fragCoord;
in vec3 Norm;


uniform sampler2D tex;
uniform sampler2D depthMap;


struct direct_light{
    vec3 direction;
    float ambent;
    vec3 diffuse;
    vec3 specular;
    float shinness;
}; 

uniform direct_light dl;
out vec4 color;
uniform vec3 eye;
in vec4 fragPosInShadowSpace;

void main()
{
    vec3 norm = normalize(Norm);
    vec3 frag2eye = normalize(eye - fragCoord.xyz);
    float diff = max(dot(normalize(-dl.direction),norm),0);
    float spec = pow(max(dot(normalize(frag2eye),normalize(reflect(dl.direction,norm))),0),dl.shinness);

    vec3 ss_xyz = fragPosInShadowSpace.xyz/fragPosInShadowSpace.w;
    ss_xyz = (ss_xyz+1.f)/2.f;
    float closestD = (texture(depthMap,ss_xyz.xy)).r;
    //float shadowBias = max(0.05 * (1.0 - dot(norm, dl.direction)), 0.005);
    float shadowBias = 0.005;
    float iS = (ss_xyz.z - shadowBias > closestD) ? 0 : 1 ;

    vec3 tcolor = (dl.ambent + iS*(diff + spec)) * vec3(texture(tex,tCoord));
    color = vec4(tcolor,1);
    //color = vec4(closestD,0,0,1);
}