#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 offset;

out vec4 _color;


void main()
{
	gl_Position = vec4(aPos + offset,0,1);
	_color = vec4(color,1);
}