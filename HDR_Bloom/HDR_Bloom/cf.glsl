#version 330 core

layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 light_color;

in VS_OUT{
	vec3 frag_in_world;
	vec2 texCoord;
	vec3 norm_in_world;
} fs_in;




uniform vec3 eye;//c

struct point_light
{
    vec3 pos;
    float constant;
    float linear;
    float quadratic;

    float ambient;
    vec3 diffuse;
    vec3 specular;
    float shinness;
};

uniform point_light red,green,blue; //c
uniform sampler2D tex; //c

vec3 calc_light(point_light pl)
{
    vec3 res;
    vec3 frag2eye = normalize(eye - fs_in.frag_in_world);
    vec3 frag2light = normalize(pl.pos - fs_in.frag_in_world);

    float diff = max(dot(frag2light,fs_in.norm_in_world),0);
    vec3 half_vec = normalize(frag2eye+frag2light);
    float spec = pow(max(dot(half_vec,fs_in.norm_in_world),0),pl.shinness);
    float dis = distance(fs_in.frag_in_world,pl.pos);
    float attenuation = 1.f/(pl.constant + dis * pl.linear + dis * pl.quadratic);

    res = attenuation * (pl.ambient + diff * pl.diffuse + spec * pl.specular) * texture(tex,fs_in.texCoord).rgb;
    return res;
}


void main()
{
	vec3 frag2eye = normalize(eye - fs_in.frag_in_world);
    vec3 color_component1 = calc_light(red);
    vec3 color_component2 = calc_light(green);
    vec3 color_component3 = calc_light(blue);

    frag_color = vec4(color_component1 ,1);
    float brightness = dot(frag_color.rgb,vec3(0.2126,0.7152,0.0722));
    if(brightness > 2.0)
        light_color = frag_color;
    else
        light_color = vec4(0,0,0,1);
}