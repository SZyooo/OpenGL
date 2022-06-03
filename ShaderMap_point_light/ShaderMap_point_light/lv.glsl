#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTCoord;
uniform mat4 model;
layout(std140,binding = 0) uniform shared_mats{
	mat4 view;
	mat4 proj;
};

void main()
{
	gl_Position = proj * view * model * vec4(aPos,1);
}