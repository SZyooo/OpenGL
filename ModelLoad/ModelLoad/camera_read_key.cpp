#include "camera_read_key.h"
#include <GLFW/glfw3.h>
#include <iostream>


camera::CameraKeyReader::CameraKeyReader(camera::Camera& p_camera)
	:callback::KeyCallback(p_camera.Name()),_camera(p_camera)
{}

void camera::CameraKeyReader::run(int key)
{
	if (key == GLFW_KEY_W)
	{
		_camera.Move(_camera.Direction(), 0.1f);
	}
	else if (key == GLFW_KEY_S)
	{
		_camera.Move(-_camera.Direction(), 0.1f);
	}
	else if (key == GLFW_KEY_A)
	{
		glm::vec3 right = glm::cross(_camera.Up(), _camera.Direction());
		_camera.Move(right, 0.1f);
	}
	else if (key == GLFW_KEY_D)
	{
		glm::vec3 left = glm::cross(_camera.Direction(), _camera.Up());
		_camera.Move(left, 0.1f);
	}
	else if (key == GLFW_KEY_0)
	{
		_camera.Reset();
	}
}
