#pragma once
#include "mousecallback.h"
#include "camera.h"
namespace camera {
	class CameraMouseReader:public callback::MouseCallback
	{
	public:
		CameraMouseReader(camera::Camera& camera);
		virtual void run(const callback::dx_dy&);
	private:
		camera::Camera& _camera;
	};
};


