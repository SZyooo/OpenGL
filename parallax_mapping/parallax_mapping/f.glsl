#version 330 core

out vec4 color;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D displaceMap;

struct direct_light
{
    vec3 direction;
    float ambent;
    vec3 diffuse;
    vec3 specular;
    float shinness;
};

uniform direct_light dl;
in GS_OUT
{
    mat3 TBN;
    vec2 tCoord;
    vec3 frag_coord_in_word;
}fs_in;
uniform vec3 eye;
uniform float height_scale;
vec2 parallaxMapping(vec2 texCoord, vec3 viewDir)
{
    float min_layers = 8;
    float max_layers = 32;
    float numLayer = mix(max_layers,min_layers,max(dot(vec3(0,0,1),viewDir),0));
    float layer_depth = 1.f / numLayer;
    float cur_depth = 0;
    vec2 P = viewDir.xy * height_scale;
    vec2 delta = P/numLayer;
    float height = texture(displaceMap,texCoord).x;
    while(cur_depth < height)
    {
        texCoord -= delta;
        cur_depth += layer_depth;
        height = texture(displaceMap,texCoord).x;
    }
    float h1 = cur_depth - texture(displaceMap,texCoord).x;
    texCoord += delta;
    float h2 = texture(displaceMap,texCoord).x - cur_depth + layer_depth;
    return texCoord - delta * (h2 / (h1 + h2));
}
void main()
{
    vec3 frag2eye = eye - fs_in.frag_coord_in_word;
    frag2eye = normalize(fs_in.TBN * frag2eye);
    vec2 rectified_tCoord = parallaxMapping(fs_in.tCoord,frag2eye);
    if(rectified_tCoord.x > 1.0 || rectified_tCoord.y > 1.0 || rectified_tCoord.x < 0 || rectified_tCoord.y < 0)
        discard;
    vec3 frag2light = -normalize(fs_in.TBN * normalize(dl.direction));
    vec3 norm = texture(normalMap,rectified_tCoord).rgb;
    norm = normalize(2.0*norm - 1);

    float diff = max(dot(frag2light,norm),0);
    vec3 half_vec = normalize(frag2eye + frag2light);
    float spec = pow(max(dot(half_vec,norm),0),dl.shinness);
    
    vec3 original_color = texture(colorMap,rectified_tCoord).rgb;
    //color = vec4(texture(displaceMap,fs_in.tCoord).rrr,1);
    color = vec4((dl.ambent +  diff*dl.diffuse + spec*dl.specular) * original_color,1);
    //color = vec4(spec * original_color,1);
}