#version 330 core

in vec2 tCoord;

//uniform sampler2D tex;

out vec4 color;
void main()
{
	//color = texture(tex,tCoord);
	color = vec4(1,0,1,1);
}