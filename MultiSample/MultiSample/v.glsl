#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 tPos;

uniform mat4 project;
uniform mat4 view;
void main()
{
	gl_Position = project * view * vec4(aPos,1);
}