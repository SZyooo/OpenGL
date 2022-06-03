#version 330 core

in vec2 texCoord;

uniform sampler2D tex;

out vec4 color;
void main()
{
	color = texture(tex,texCoord);
	/*float gama = 2.2;
	color.r = pow(color.r,1/gama);
	color.g = pow(color.g,1/gama);
	color.b = pow(color.b,1/gama);*/
}