#version 430
out vec4 color;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;


//接受同名的数据（在顶点着色器中的varyingColor）
in vec4 varyingColor;

void main(void)
{
	//color = vec4(1.0,0.0,0.0,1.0);
	color = varyingColor;
}