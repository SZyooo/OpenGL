#version 330 core

in vec3 Norm;
uniform samplerCube cube;
in vec3 fragPos;
out vec4 color;

void main()
{
	vec3 refl = reflect(fragPos,Norm);
	vec3 refr = refract(fragPos,Norm,0.5f);
	color = texture(cube,refl);
}
