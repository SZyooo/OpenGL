#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<memory>
#include<string>
namespace init {
	std::shared_ptr<GLFWwindow> initWindow
	(int screen_width, int screen_height, std::string window_name)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GLFWwindow* pwindow = glfwCreateWindow(screen_width, screen_height, window_name.c_str(), nullptr, nullptr);
		std::shared_ptr<GLFWwindow> window(pwindow);
		if (window.get() == nullptr)
		{
			glfwTerminate();
			throw "init glfw failed";
		}
		glfwMakeContextCurrent(window.get());
		if (glewInit() != GLEW_OK)
		{
			throw "init glew failed";
		}
		return window;
	}
};
