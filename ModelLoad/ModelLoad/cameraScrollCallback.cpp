#include "cameraScrollCallback.h"

camera::CameraScrollReader::CameraScrollReader(camera::Camera& camera)
	:callback::ScrollCallback(camera.Name()),_camera(camera)
{}

void camera::CameraScrollReader::run(float dx, float dy)
{
	_camera.Zoom(dy*0.01);
}
