#version 330 core

in vec2 tCoord;
uniform sampler2D tex;
out vec4 color;
void main()
{
	//color = vec4(texture(tex,tCoord).rgb,1);
	color = texture(tex,tCoord);
	//color = vec4(0,0,0,1);
}