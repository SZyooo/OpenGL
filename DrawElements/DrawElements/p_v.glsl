#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 texCoord;

out vec2 tCoord;

void main()
{
	tCoord = texCoord;
	gl_Position = vec4(aPos.xy,0,1);
}