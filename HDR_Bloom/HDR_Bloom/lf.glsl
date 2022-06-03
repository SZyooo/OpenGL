#version 330 core
uniform vec3 light_color;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 bright_color;

void main()
{
	color = vec4(light_color,1);
	float brightness = dot(light_color,vec3(0.2126,0.7152,0.0722));
    bright_color = color;
}