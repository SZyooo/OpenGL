#version 330 core

in vec2 _tex_coord;

uniform sampler2D tex;
out vec4 color;
struct pLight{
	vec3 pos;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shinness;
};

in vec3 norm;
uniform pLight light;
uniform vec3 eye;
in vec3 fragPos;
void main()
{
    vec3 _color = texture(tex,_tex_coord).xyz;
    vec3 f2l = normalize(light.pos - fragPos);
    
    float diff = max(dot(norm,f2l),0);
    //vec3 lightDiffuse = diff;//* light.diffuse;

    vec3 f2e = normalize(eye-fragPos);
    float spec = pow(max(dot(f2e,reflect(-f2l,norm)),0),light.shinness);
    //vec3 lightSpecular = spec * light.specular;

    float dis = distance(light.pos,fragPos);
    //float attenuation = 1.0f/(light.constant + light.linear * dis + light.quadratic * dis*dis);

    //color = vec4(attenuation*(light.ambient+lightDiffuse+lightSpecular)*texture(tex, _tex_coord).xyz,1);
    color = vec4((spec + diff)*_color + 0.05*_color,1);
}