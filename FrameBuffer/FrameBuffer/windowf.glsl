#version 330 core

in vec2 texCoord;
uniform sampler2D tex;

out vec4 color;
void main()
{
	 vec4 ocolor = vec4(texture(tex,texCoord).rgb,1.f);
	 float avg = (ocolor.x + ocolor.y + ocolor.z) / 3;
	 color = vec4(avg,avg,avg,1);
}