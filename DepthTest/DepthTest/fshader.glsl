#version 330 core

in vec2 texCoord;

uniform sampler2D _texture;
out vec4 color;

void main()
{
	vec4 tcolor = texture(_texture,texCoord);
	if(tcolor.a < 0.1)
		discard;
	color = tcolor;
	//color = vec4(1,1,0,1);
}