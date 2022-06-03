#version 440 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 tCoord;

uniform mat4 view;

uniform mat4 project;

out vec2 tex_coord;
void main()
{
	tex_coord = tCoord;
	gl_Position = project * view * vec4(aPos,1);
}
