#include "processInput.h"
#include "camerasController.h"
#include <iostream>

void input::mouse_callback(GLFWwindow* window, double x, double y)
{
	input::Input& instance = input::Input::Instance();
	double dx = x - instance._last_mouse_pos_x;
	double dy = y - instance._last_mouse_pos_y;
	std::cout << instance._last_mouse_pos_x << std::endl;
	instance._last_mouse_pos_x = x;
	instance._last_mouse_pos_y = y;
	for (auto& reader : instance._mouse_readers)
	{
		reader.second->run({ dx,dy });
	}
}

void input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	input::Input& instance = input::Input::Instance();
	for (auto& reader : instance._key_readers)
	{
		reader.second->run(key);
	}
}

void input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	input::Input& instance = input::Input::Instance();
	for (auto& reader : instance._scroll_readers)
	{
		reader.second->run(xoffset, yoffset);
	}
}

void input::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void input::Input::RegisterKeyCallback(std::shared_ptr<callback::KeyCallback>& reader)
{
	if (INSTANCE._key_readers.find(reader->Name()) == INSTANCE._key_readers.end())
	{
		INSTANCE._key_readers.insert({ reader->Name(), reader });
	}
}

void input::Input::RegisterMouseCallBack(std::shared_ptr<callback::MouseCallback>& reader)
{
	if (INSTANCE._mouse_readers.find(reader->Name()) == INSTANCE._mouse_readers.end())
	{
		INSTANCE._mouse_readers.insert({ reader->Name(),reader });
	}
}

void input::Input::RegisterScrollCallBack(std::shared_ptr<callback::ScrollCallback>& reader)
{
	if (INSTANCE._scroll_readers.find(reader->Name()) == INSTANCE._scroll_readers.end())
	{
		INSTANCE._scroll_readers.insert({ reader->Name(), reader });
	}
}

input::Input& input::Input::Instance()
{
	if (INSTANCE._window.use_count() == 0)
		throw "input is not allocated a window";
	return INSTANCE;
}

void input::Input::setWindow(std::shared_ptr<GLFWwindow>& window)
{
	input::Input::INSTANCE._window = window;
	glfwSetCursorPosCallback(window.get(), mouse_callback);
	glfwSetKeyCallback(window.get(), key_callback);
	glfwSetScrollCallback(window.get(), scroll_callback);
	glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);
}

input::Input::Input()
{}

input::Input input::Input::INSTANCE;
