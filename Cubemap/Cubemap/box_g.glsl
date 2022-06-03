#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;


in VS_OUT{
	vec2 texCoord;
} gs_in[];
out vec2 _texCoord;

vec3 gen_norm()
{
	vec3 a = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	vec3 b = gl_in[0].gl_Position.xyz - gl_in[2].gl_Position.xyz;
	return normalize(cross(a,b));
}

uniform float time;
vec4 explode(vec4 pos,vec3 norm)
{
	float magnitude = 2;
	vec4 new_pos = pos + (sin(time) + 1)/2*vec4(norm,0) * magnitude;
	return new_pos;
}
void main()
{
	vec3 norm = gen_norm();
	gl_Position = explode(gl_in[0].gl_Position,norm);
	_texCoord = gs_in[0].texCoord;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position,norm);
	_texCoord = gs_in[1].texCoord;
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position,norm);
	_texCoord = gs_in[2].texCoord;
	EmitVertex();
	EndPrimitive();
}
