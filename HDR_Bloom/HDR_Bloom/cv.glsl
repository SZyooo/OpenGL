#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 view;//c
uniform mat4 model;//c
uniform mat4 proj;//c

out VS_OUT{
	vec3 frag_in_world;
	vec2 texCoord;
	vec3 norm_in_world;
} vs_out;

void main()
{
	vec3 to_world = vec3(model * vec4(aPos,1));
	gl_Position = proj * view * vec4(to_world,1);
	vs_out.frag_in_world = to_world;
	vs_out.texCoord = aTexCoord;
	vs_out.norm_in_world = normalize(vec3(transpose(inverse(model)) * vec4(aNorm,0)));
}