#version 330 core
layout (points) in;
layout (triangle_strip,max_vertices = 5) out;


in VS_OUT{
	vec3 _color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 pos)
{	
	gl_Position = pos + vec4(-0.2,-0.2,0,0);
	EmitVertex();
	gl_Position = pos + vec4(0.2,-0.2,0,0);
	EmitVertex();
	gl_Position = pos + vec4(-0.2,0.2,0,0);
	EmitVertex();
	gl_Position = pos + vec4(0.2,0.2,0,0);
	EmitVertex();
	fColor = vec3(1,1,1);
	gl_Position = pos + vec4(0,0.4,0,0);
	EmitVertex();
	EndPrimitive();
}


void main()
{
	fColor = gs_in[0]._color;
	build_house(gl_in[0].gl_Position);
}
