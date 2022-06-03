#version 330 core

struct material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shinness;
};
out vec4 fragColor;
uniform material mat;

void main()
{
	fragColor = vec4(mat.diffuse,1);
}