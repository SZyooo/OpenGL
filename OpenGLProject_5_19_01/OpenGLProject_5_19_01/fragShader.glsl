#version 430
out vec4 color;
void main(void)
{
	if(gl_FragCoord.x < 0)
	{
		color = vec4(1.0,0.0,0.0,1.0);
	}
	else
	{
		color = vec4(0.0,0.0,1.0,1.0);
	}
}
mat4 buildTraslate(float x,float y,float z)
{
	mat4 trans = mat4(1,0,0,0,0,0,0,0,0,0,1,0,x,y,z,1);
	return trans;
}
mat4 buildRotateY(float rad)
{
	mat4 yrot = mat4(cos(rad),0,sin(rad),0,0,1,0,0,-sin(rad),0,cos(rad),0,0,0,0,1);
	return yrot;
}
mat4 buildRotZ(float rad)
{
	mat4 zrot = mat4(cos(rad),-sin(rad),0,0,sin(rad),cos(rad),0,0,0,0,1,0,0,0,0,1);
	return zrot;
}
mat4 buildScale(float x,float y, float z)
{
	mat4 scale = mat4(x,0,0,0,0,y,0,0,0,0,z,0,0,0,0,1);
	return scale;
}