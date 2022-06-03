

#include"model.h"
#include "camerasController.h"
#include<GLFW/glfw3.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "program.h"
#include "initiate.h"
#include "modelTools.h"
#include "processInput.h"
#include "camera_read_key.h"
#include "cameraMouseReader.h"
#include "cameraScrollCallback.h"


int main() {
	std::shared_ptr<GLFWwindow> window = init::initWindow(800, 600, "opengl");
	model::Model backpack;
	model::load_model(backpack, "E:\\3dmodels\\lollyHouse\\11-obj\\.OBJ\\Room #1.obj", "backpack_v.glsl", "backpack_f.glsl");
	camera_controller::cameraController& instance = camera_controller::cameraController::Instance();
	instance.CreateNewCamera("main camera");
	instance.ActivateCamera("main camera");
	camera::Camera camera = instance.ActiveCamera();
	std::shared_ptr<callback::KeyCallback> keyReader(new camera::CameraKeyReader(camera));
	std::shared_ptr<callback::MouseCallback> mouseReader(new camera::CameraMouseReader(camera));
	std::shared_ptr<callback::ScrollCallback> scrollReader(new camera::CameraScrollReader(camera));
	input::Input::setWindow(window);
	input::Input::Instance().RegisterKeyCallback(keyReader);
	input::Input::Instance().RegisterMouseCallBack(mouseReader);
	input::Input::Instance().RegisterScrollCallBack(scrollReader);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window.get()))
	{
		glClearColor(0.5f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 project
			= glm::perspective(camera.FOV(), 800.f / 600.f, camera.NearPlane(), camera.FarPlane());
		glm::mat4 view = camera.LookAtMat();
		backpack.Draw(view, project);
		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}
	return 0;
	
}



