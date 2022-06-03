#version 330 core

in GS_OUT
{
	vec3 global_pos;
	mat3 tbn;
    vec2 tCoord;
}fs_in;

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

uniform point_light red;
uniform vec3 eye;
uniform sampler2D tex;
uniform sampler2D normal;

out vec4 color;

void main()
{
    vec3 frag2eye = normalize(eye - fs_in.global_pos);
    vec3 frag2light = normalize(red.pos - fs_in.global_pos);
    frag2eye = normalize(fs_in.tbn * frag2eye);
    frag2light = normalize(fs_in.tbn * frag2light);
    vec3 basic_color = texture(tex,fs_in.tCoord).rgb;
    vec3 norm = texture(normal,fs_in.tCoord).xyz;
    vec3 halfway = normalize(frag2eye+frag2light);

    float spec = max(dot(frag2light,norm),0);
    float diff = pow(max(dot(halfway,frag2eye),0),red.shinness);

    float dis = distance(red.pos,fs_in.global_pos);
    float attenuation = red.constant + red.linear * dis + red.quadratic * dis*dis;
    attenuation = 1/attenuation;

    vec3 t_color = red.ambient * basic_color + red.diffuse * diff * basic_color + red.specular * spec * basic_color;
    t_color = attenuation * t_color;
    t_color = t_color/(t_color+1);
    color = vec4(t_color,1);

}

