#include "camerasController.h"



camera_controller::cameraController& camera_controller::cameraController::Instance()
{
	return INSTANCE;
}

void camera_controller::cameraController::CreateNewCamera(std::string cam_name, camera::v3 pos, camera::v2 rotate, camera::v3 up,float nd,float fd,float fov,float ms,float rs)
{
	for (auto& cam : this->cameras)
	{
		if (cam.Name() == cam_name)
		{
			throw "Cannot add two cameras with a same name in a cameraController";
		}
	}
	camera::Camera new_camera(cam_name,pos, rotate, up,nd,fd,fov,ms,rs);
	this->cameras.push_back(new_camera);
	if (this->cameras.size() == 1)
	{
		this->activeCamera = 0;
	}
}

void camera_controller::cameraController::DeleteCamera(std::string cam_name)
{
	if (cameras.size() == 0)
	{
		return;
	}
	std::vector<camera::Camera>::iterator it;
	bool find = false;
	int idx = -1;
	for (size_t i = 0; i < cameras.size(); i++)
	{
		if (cameras[i].Name() == cam_name)
		{
			it = cameras.begin() + i;
			find = true;
			idx = i;
			break;
		}
	}
	if (find)
	{
		cameras.erase(it);
		if (idx == activeCamera)
		{
			if (cameras.size() == 0)
			{
				activeCamera = -1;
			}
			else
				activeCamera = 0;
		}
	}
}

camera::Camera& camera_controller::cameraController::ActiveCamera()
{
	if (activeCamera == -1)
		throw "no camera";
	else {
		return cameras[activeCamera];
	}
}

void camera_controller::cameraController::ActivateCamera(std::string name)
{
	for (size_t i = 0; i < this->cameras.size(); i++)
	{
		if (cameras[i].Name() == name)
		{
			this->activeCamera = i;
		}
	}
}

camera_controller::cameraController::cameraController()
{
	activeCamera = -1;
}

camera_controller::cameraController camera_controller::cameraController::INSTANCE;