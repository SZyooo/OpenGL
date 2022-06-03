#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tCoord;

uniform mat4 model;

uniform mat4 to_shadow_space;

void main()
{
	gl_Position = to_shadow_space * model * vec4(aPos,1);
}