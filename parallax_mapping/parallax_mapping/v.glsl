#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT
{
	vec3 _vertex_pos;
	vec2 _texture_coord;
} vs_out;

void main()
{
	vs_out._vertex_pos = vec3(aPos,0);
	vs_out._texture_coord = aTCoord;
	gl_Position = proj * view * model * vec4(aPos,0,1);
}