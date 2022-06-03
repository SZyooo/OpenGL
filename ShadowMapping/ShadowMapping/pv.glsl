#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 _norm;
layout(location = 2) in vec2 texCoord;

out vec2 tCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
uniform mat4 to_shadow_space;

out vec3 fragFoord;
out vec3 Norm;
out vec4 fragPosInShadowSpace;

void main()
{
	vec4 posInWord = model * vec4(aPos,1);
	Norm = mat3(transpose(inverse(model)))*_norm;
	gl_Position =project * view * posInWord;
	tCoord = texCoord;
	fragFoord = posInWord.xyz;
	fragPosInShadowSpace = to_shadow_space * posInWord;
}