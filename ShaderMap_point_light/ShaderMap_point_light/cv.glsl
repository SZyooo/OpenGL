#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTCoord;


uniform mat4 model;
layout(std140,binding = 0) uniform shared_mats{
	mat4 view;
	mat4 proj;
};


out vs_out
{
	vec3 frag_norm;
	vec3 frag_pos;
	vec2 tCoord;
} _vs_out;

void main()
{
	_vs_out.frag_norm = vec3(transpose(inverse(model)) * vec4(aNorm,0));
	vec4 f_pos = model * vec4(aPos,1);
	_vs_out.frag_pos = vec3(f_pos);
	gl_Position = proj * view * f_pos;
	_vs_out.tCoord = aTCoord;
}