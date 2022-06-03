#version 330 core

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
void main()
{
	fragColor = texture(diffuse0,texCoords);
	//fragColor = vec4(texCoords,1,1);
}