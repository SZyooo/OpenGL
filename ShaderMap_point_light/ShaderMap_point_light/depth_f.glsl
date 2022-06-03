#version 440 core

in vec3 frag_pos;
uniform vec3 light_pos;
uniform float far;

void main()
{
	float dis = distance(light_pos,frag_pos);
	dis = dis / far;
	gl_FragDepth = dis;
}