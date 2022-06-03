#version 330 core

layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;
uniform mat4 model;


in VS_OUT
{
	vec3 pos_in_world;
	vec3 pos;
	vec2 texCoord;
} gs_in[];

out GS_OUT
{
	vec3 global_pos;
	mat3 tbn;
	vec2 tCoord;
}gs_out;

void main()
{
	vec3 e1 = gs_in[1].pos - gs_in[0].pos;
	vec3 e2 = gs_in[2].pos - gs_in[0].pos;
	vec2 dUV1 = gs_in[1].texCoord - gs_in[0].texCoord;
	vec2 dUV2 = gs_in[2].texCoord - gs_in[0].texCoord;
	float determ = dUV1.x * dUV2.y - dUV1.y * dUV2.x;
	vec3 T,B;
	T.x = dUV2.y*e1.x - dUV1.y*e2.x;
	T.y = dUV2.y*e1.y - dUV1.y*e2.y;
	T.z = dUV2.y*e1.z - dUV1.y*e2.z;
	T = T / determ;
	B.x = -dUV2.x * e1.x + dUV1.x *e2.x;
	B.y = -dUV2.x * e1.y + dUV1.x *e2.y;
	B.z = -dUV2.x * e1.z + dUV1.x *e2.z;
	B = B / determ;
	T = normalize(mat3(model) * T);
	B = normalize(mat3(model) * B);
	vec3 N = normalize(cross(T,B));
	gs_out.tbn = inverse(mat3(T,B,N));
	gl_Position = gl_in[0].gl_Position;
	gs_out.global_pos = gs_in[0].pos_in_world;
	gs_out.tCoord = gs_in[0].texCoord;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	gs_out.global_pos = gs_in[1].pos_in_world;
	gs_out.tCoord = gs_in[1].texCoord;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	gs_out.global_pos = gs_in[2].pos_in_world;
	gs_out.tCoord = gs_in[2].texCoord;
	EmitVertex();
	EndPrimitive();
}