#pragma once
#include "camera.h"
#include "scrollcallback.h"


namespace camera {
	class CameraScrollReader :public callback::ScrollCallback
	{
	public:
		CameraScrollReader(camera::Camera& camera);
		virtual void run(float vd, float hd);
	private:
		camera::Camera& _camera;
	};
};