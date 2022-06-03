#pragma once
#include <GLFW/glfw3.h>
#include <map>
#include <memory>
#include "keycallback.h"
#include "mousecallback.h"
#include "scrollcallback.h"



namespace input {
	using dx_dy = std::pair<double, double>;
	class Input {
	public:
		void RegisterKeyCallback(std::shared_ptr<callback::KeyCallback>&);
		void RegisterMouseCallBack(std::shared_ptr<callback::MouseCallback>&);
		void RegisterScrollCallBack(std::shared_ptr<callback::ScrollCallback>&);
		static Input& Instance();
		static void setWindow(std::shared_ptr<GLFWwindow>& window);
	private:
		Input();
		friend void mouse_callback(GLFWwindow*, double, double);
		friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		double _last_mouse_pos_x, _last_mouse_pos_y;
		std::shared_ptr<GLFWwindow> _window;
		std::map<std::string,std::shared_ptr<callback::KeyCallback>> _key_readers;
		std::map<std::string,std::shared_ptr<callback::MouseCallback>> _mouse_readers;
		std::map<std::string, std::shared_ptr<callback::ScrollCallback>> _scroll_readers;
		static Input INSTANCE;
	};
};