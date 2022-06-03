#include "CameraMouseReader.h"

camera::CameraMouseReader::CameraMouseReader(camera::Camera& camera)
	: callback::MouseCallback(camera.Name()),_camera(camera)
{
}

void camera::CameraMouseReader::run(const callback::dx_dy& dxdy)
{
	//std::cout << "mouse dx = " << dxdy.first << " dy = " << dxdy.second << std::endl;
	_camera.RotateHorizontally(-dxdy.first);
	_camera.RotateVeticlally(dxdy.second);
}
