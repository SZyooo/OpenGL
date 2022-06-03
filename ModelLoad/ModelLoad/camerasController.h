#pragma once
#include "camera.h"
#include <vector>

namespace camera_controller {
	class cameraController {
	public:
		static cameraController& Instance();
		void CreateNewCamera(std::string cam_name, camera::v3 pos = camera::v3(0, 0, 0), camera::v2 rotate = camera::v2(0, 0), camera::v3 up = camera::v3(0, 1, 0), float nd = 0.1, float fd = 100,float fov = 45., float ms = 1., float rs = 0.5);
		void DeleteCamera(std::string cam_name);
		camera::Camera& ActiveCamera();
		void ActivateCamera(std::string name);
	private:
		cameraController();
		static cameraController INSTANCE;
		std::vector<camera::Camera> cameras;
		int activeCamera;
	};
};