#version 330 core

in vs_out
{
	vec3 frag_norm;
	vec3 frag_pos;
    vec2 tCoord;
} _fs_in;
uniform vec3 eye_pos;
uniform sampler2D tex;
uniform samplerCube cube_tex;
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
uniform point_light pl;
out vec4 color;

uniform float far;
float cal_shadow()
{
    float shadow = 0;
    vec3 vec = _fs_in.frag_pos - pl.pos;
    float dis = length(vec);
    float bias = 0.05;
    float offset = 0.01;
    for(float x = -offset;x <=offset;x+=offset)
    {
        for(float y = -offset; y <= offset; y+=offset)
        {
            for(float z = -offset;z<=offset;z+=offset)
            {
                float occlude_dis = texture(cube_tex,vec + vec3(x,y,z)).r;
                occlude_dis *= far;
                if((occlude_dis + bias) <= dis)
                    shadow += 1;
            }
        }
    }

    return shadow/27.f;
}

void main()
{
	vec3 light_to_eye = normalize(eye_pos - pl.pos);
    vec3 light_to_frag = normalize(_fs_in.frag_pos - pl.pos);
    vec3 frag_to_eye = normalize(eye_pos - _fs_in.frag_pos);
    vec3 middle_norm = normalize(normalize(frag_to_eye) - normalize(light_to_frag));
    
    
    float diff = max(dot(light_to_eye,_fs_in.frag_norm),0);
    float spec = pow(max(dot(_fs_in.frag_norm,middle_norm),0),pl.shinness);
    float dis = distance(_fs_in.frag_pos,pl.pos);
    float attenuation = 1/(pl.constant + pl.linear * dis + pl.quadratic * dis * dis);
    vec3 t_diffuse = diff * pl.diffuse;
    vec3 t_specular = spec * pl.specular;
    vec3 t_color = vec3(texture(tex,_fs_in.tCoord));
    float shadow = cal_shadow();
    vec3 sum_color = pl.ambient * t_color +(1-shadow) * (t_diffuse + t_specular)*t_color;
    sum_color *= attenuation;
    color = vec4(sum_color,1.f);
    //color = texture(tex,_fs_in.tCoord);
}