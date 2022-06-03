#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 tCoord;

out vec2 _tex_coord;

uniform mat4 project;
uniform mat4 view;
uniform mat4 model;
out vec3 norm;
out vec3 fragPos;
void main()
{
	fragPos = (model * vec4(aPos,0,1)).xyz;
	gl_Position = project * view * vec4(fragPos,1);
	_tex_coord = tCoord;
	norm = normalize((transpose(inverse(model)) * vec4(0,0,1,0)).xyz);
}