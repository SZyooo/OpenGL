#pragma once
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
namespace program {
	class Program
	{
	public:
		Program(std::string v = "defaultVShader.glsl", std::string f = "defaultFShader.glsl");
		GLuint ID() const { return _id; }
		void Use()
		{
			glUseProgram(_id);
		}
		void SetBool(std::string uniformName, bool val)
		{
			GLint loc = glGetUniformLocation(_id, uniformName.c_str());
			Use();
			glUniform1i(loc, (int)val);
		}
		void SetInt(std::string uniformName, int val)
		{
			GLint loc = glGetUniformLocation(_id, uniformName.c_str());
			Use();
			glUniform1i(loc, val);
		}
		void SetFloat(std::string uniformName, float val)
		{
			GLint loc = glGetUniformLocation(_id, uniformName.c_str());
			Use();
			glUniform1f(loc, val);
		}
		void SetMat4(std::string uniformName, glm::mat4 m4)
		{
			GLint loc = glGetUniformLocation(_id, uniformName.c_str());
			Use();
			glUniformMatrix4fv(loc, 1, GL_FALSE, &m4[0][0]);
		}
		std::string VertexShaderPath() const { return this->_vertexShaderPath; }
		std::string FragmentShaderPath() const { return this->_fragmentShaderPath; }
	private:
		GLuint _id;
		std::string _vertexShaderPath;
		std::string _fragmentShaderPath;
	};
};