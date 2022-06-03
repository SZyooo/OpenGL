#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>

class Shader
{
private:
	GLuint programID;
	GLuint vertexShaderID, FragmentShaderID;
	bool success;
public:
	Shader(const char* vertexFilePath, const char* fragmentFilePath);
	bool useProgram();
	
};

