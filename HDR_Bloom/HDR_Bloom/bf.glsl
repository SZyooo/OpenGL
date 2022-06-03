#version 330 core

uniform sampler2D tex;
uniform bool vertical;

in vec2 tCoord;
uniform float weight[5] = float[](0.2270270270,0.194595946,0.1216216216,0.0540540541,0.0162162162);

out vec4 color;
void main()
{
	vec2 tex_offset = 1.0/textureSize(tex,0);
	vec3 res = texture(tex,tCoord).rgb * weight[0];
	if(vertical)
	{
		for(int i=1;i<5;i++)
		{
			res += texture(tex,tCoord + vec2(0.0,tex_offset.y * i)).rgb * weight[i];
			res += texture(tex,tCoord - vec2(0.0,tex_offset.y * i)).rgb * weight[i];
		}
	}
	else{
		for(int i=1;i<5;i++)
		{
			res += texture(tex,tCoord + vec2(tex_offset.x * i,0.0)).rgb * weight[i];
			res += texture(tex,tCoord - vec2(tex_offset.x * i,0.0)).rgb * weight[i];
		}
	}
	color = vec4(res,1);
}