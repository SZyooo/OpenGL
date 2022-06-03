#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 project;
//uniform mat4 model;
out vec3 tCoord;
void main()
{
	tCoord = aPos;
	vec4 pos = project * mat4(mat3(view)) * vec4(aPos,1.0f);
	gl_Position = pos.xyww;
}