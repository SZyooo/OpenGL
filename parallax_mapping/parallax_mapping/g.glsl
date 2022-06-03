#version 330 core

layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;

uniform mat4 model;
in VS_OUT
{
	vec3 _vertex_pos;
	vec2 _texture_coord;
} gs_in[];

out GS_OUT
{
	mat3 TBN;
	vec2 tCoord;
	vec3 frag_coord_in_word;
} gs_out;

void main()
{
	vec3 e1 = gs_in[1]._vertex_pos - gs_in[0]._vertex_pos;
	vec3 e2 = gs_in[2]._vertex_pos - gs_in[0]._vertex_pos;

	vec3 norm = normalize(cross(e1,e2));
	float dU1 = gs_in[1]._texture_coord.x - gs_in[0]._texture_coord.x;
	float dV1 = gs_in[1]._texture_coord.y - gs_in[0]._texture_coord.y;
	float dU2 = gs_in[2]._texture_coord.x - gs_in[0]._texture_coord.x;
	float dV2 = gs_in[2]._texture_coord.y - gs_in[0]._texture_coord.y;

	float determ = 1.f/(dU1*dV2 - dU2*dV1);
	vec3 T,B,N;
	T.x = determ * (dV2 * e1.x - dV1 * e2.x);
	T.y = determ * (dV2 * e1.y - dV1 * e2.y);
	T.z = determ * (dV2 * e1.z - dV1 * e2.z);
	B.x = determ * (-dU2 * e1.x + dU1 * e2.x);
	B.y = determ * (-dU2 * e1.y + dU1 * e2.y);
	B.z = determ * (-dU2 * e1.z + dU1 * e2.z);
	//mat4 normal_mat = transpose(inverse(model));
	T = normalize(vec3(model * vec4(T,0)));
	B = normalize(vec3(model * vec4(B,0)));
	//N = normalize(cross(T,B));
	N = normalize(vec3(model * vec4(norm,0)));

	gs_out.TBN = inverse(mat3(T,B,N));
	gl_Position = gl_in[0].gl_Position;
	gs_out.tCoord = gs_in[0]._texture_coord;
	gs_out.frag_coord_in_word = vec3(model * vec4(gs_in[0]._vertex_pos,1));
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	gs_out.tCoord = gs_in[1]._texture_coord;
	gs_out.frag_coord_in_word = vec3(model * vec4(gs_in[1]._vertex_pos,1));
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	gs_out.tCoord = gs_in[2]._texture_coord;
	gs_out.frag_coord_in_word = vec3(model * vec4(gs_in[2]._vertex_pos,1));
	EmitVertex();
	EndPrimitive();

}