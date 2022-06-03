#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 tCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 world_pos;
out vec2 v_tCoord;
void main()
{
	v_tCoord = tCoord;
	vec4 pos_in_world = model * vec4(aPos,0,1);
	world_pos = vec3(pos_in_world);
	gl_Position = proj * view * pos_in_world;
}