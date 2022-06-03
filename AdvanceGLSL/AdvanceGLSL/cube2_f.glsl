#version 440 core

in vec2 tex_coord;

uniform sampler2D outer;
uniform sampler2D inner;

out vec4 color;
void main()
{
	if(gl_FrontFacing)
	{
		color = texture(outer,tex_coord);
	}
	else{
		color = texture(inner,tex_coord);
	}
}