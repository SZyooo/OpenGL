#version 440 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;
uniform mat4 to_shadow_spaces[6];

out vec3 frag_pos;

void main()
{
	for(int face = 0; face < 6; face ++)
	{
		gl_Layer = face;
		for(int v = 0; v < 3; v++)
		{
			frag_pos = gl_in[v].gl_Position.xyz;
			gl_Position = to_shadow_spaces[face] * gl_in[v].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
}

