#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;

out vec2 tCoord;

uniform mat4 model;
uniform mat4 project;
uniform mat4 view;
void main()
{
	tCoord = texCoord;
	gl_Position =project *  view * model * vec4(aPos,1);
}