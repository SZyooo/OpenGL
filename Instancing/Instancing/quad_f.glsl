#version 330 core

out vec4 fColor;
in vec4 _color;

void main()
{
	fColor = _color;
}