#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 norm;

uniform mat4 project;
uniform mat4 view;
uniform mat4 model;

out vec3 Norm;
out vec3 fragPos;
void main()
{
	mat4 mv = view * model;
	vec4 posInViewSpace = mv * vec4(aPos,1.0);
	fragPos = posInViewSpace.xyz;
	gl_Position = project * posInViewSpace;
	Norm = vec3(transpose(inverse(mv)) * vec4(norm,0));
}