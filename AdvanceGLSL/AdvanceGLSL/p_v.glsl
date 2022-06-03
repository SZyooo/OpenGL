#version 440 core
layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 project;


void main()
{
	gl_Position = project * view * vec4(aPos,1);
	gl_PointSize = gl_Position.z * 10;
}