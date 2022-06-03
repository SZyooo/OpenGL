#version 330 core

layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;

in vec3 world_pos[];
in vec2 v_tCoord[];

out vec2 g_tCoord;
out vec3 g_pos;
out vec3 world_T;
out vec3 world_B;
out vec3 world_N;

uniform mat4 model;

void main()
{
	vec3 v1 = world_pos[1] - world_pos[0];
	vec3 v2 = world_pos[2] - world_pos[0];
	//notice that the norm has been in world space
	vec3 norm = normalize(cross(v1,v2));

	//calculate TBN
	float deltaU1 = v_tCoord[1].x - v_tCoord[0].x;
	float deltaV1 = v_tCoord[1].y - v_tCoord[0].y;
	float deltaU2 = v_tCoord[2].x - v_tCoord[0].x;
	float deltaV2 = v_tCoord[2].y - v_tCoord[0].y;

	float determ = 1.f/(deltaU1*deltaV2 - deltaU2*deltaV1);

	//transpose adjacent matrix 
	//¦¤v2	-¦¤v1
	//-¦¤u2	¦¤u1
	vec3 T,B;
	T.x = determ * (deltaV2*v1.x - deltaV1*v2.x);
	T.y = determ * (deltaV2*v1.y - deltaV1*v2.y);
	T.z = determ * (deltaV2*v1.z - deltaV1*v2.z);

	B.x = determ * (-deltaU2*v1.x + deltaU1*v2.x);
	B.y = determ * (-deltaU2*v1.y + deltaU1*v2.y);
	B.z = determ * (-deltaU2*v1.z + deltaU1*v2.z);

	mat3 norm_matrix = transpose(inverse(mat3(model)));
	T = vec3(norm_matrix * T);
	B = vec3(norm_matrix * B);

	world_B = B;
	world_N = norm;
	world_T = T;
	g_tCoord = v_tCoord[0];
	g_pos = world_pos[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	g_tCoord = v_tCoord[1];
	g_pos = world_pos[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	g_tCoord = v_tCoord[2];
	g_pos = world_pos[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	EndPrimitive();
}