#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 tCoord;



out VS_OUT{
	vec2 texCoord;
} vs_out;

uniform mat4 view;
uniform mat4 project;

void main()
{
	vs_out.texCoord = tCoord;
	gl_Position = project * view * vec4(aPos,1.f);
}