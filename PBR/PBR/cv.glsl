#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT
{
	vec3 pos_in_world;
	vec3 pos;
	vec2 texCoord;
} vs_out;


void main()
{
	vs_out.pos = aPos;
	vs_out.pos_in_world = vec3(model * vec4(aPos,1));
	gl_Position = proj * view * vec4(vs_out.pos_in_world,1);
	vs_out.texCoord = aTexCoord;
}