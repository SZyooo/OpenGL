#pragma once
#include "keycallback.h"
#include "camera.h"

namespace camera {
	class CameraKeyReader :public callback::KeyCallback
	{
	public:
		CameraKeyReader(camera::Camera& camera);
		void virtual run(int key);
	private:
		Camera& _camera;
	};
};