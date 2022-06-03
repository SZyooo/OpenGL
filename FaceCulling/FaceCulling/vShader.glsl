#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 tCoord;

uniform mat4 view;
uniform mat4 project;
uniform mat4 model;
out vec2 texCoord;
void main()
{
	texCoord = tCoord;
	gl_Position = project * view * model * vec4(aPos,1);
}

