#version 330 core

uniform sampler2D tex;
uniform sampler2D bloom;

in vec2 tCoord;

out vec4 color;
void main()
{
	float exposure = 5;
	vec3 rgb = texture(tex,tCoord).rgb;
	rgb += texture(bloom,tCoord).rgb;
	rgb = rgb / (rgb + vec3(1));
	//rgb = pow(rgb,vec3(1.0 / 2.2));
	color = vec4(rgb,1);
	//color = vec4(texture(tex,tCoord).rgb,1);
	//color = vec4(0.1,0.2,0.4,1);
}