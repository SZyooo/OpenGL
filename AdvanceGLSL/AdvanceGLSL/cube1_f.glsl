#version 330 core


out vec4 color;

void main()
{
	if(gl_FragCoord.x < 400)
		color = vec4(0.5,0.2,0.3,1);
	else
		color = vec4(0.3,0.7,0.1,1);
}