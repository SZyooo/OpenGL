#include "Shader.h"
Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath) {
	success = true;
	std::ifstream in(vertexFilePath);
	std::stringstream stream;
	stream << in.rdbuf();
	in.close();
	std::string code = stream.str();
	const char* chr_code = code.c_str();
	this->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->vertexShaderID, 1, &chr_code, NULL);
	glCompileShader(this->vertexShaderID);
	int error;
	glGetShaderiv(this->vertexShaderID, GL_COMPILE_STATUS, &error);
	if (!error)
	{
		success = false;
		char msg[128];
		glGetShaderInfoLog(this->vertexShaderID, 128, NULL, msg);
		std::cout << "COMPILE VERTEX SHADER ERROR:\nFILE PATH:" << vertexFilePath << "\nError:" << msg << std::endl;
	}
	stream.str("");
	in.open(fragmentFilePath);
	stream << in.rdbuf();
	code = stream.str();
	chr_code = code.c_str();
	this->FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->FragmentShaderID, 1, &chr_code, NULL);
	glCompileShader(this->FragmentShaderID);
	glGetShaderiv(this->FragmentShaderID, GL_COMPILE_STATUS, &error);
	if (!error)
	{
		success = false;
		char msg[128];
		glGetShaderInfoLog(this->FragmentShaderID, 128, NULL, msg);
		std::cout << "COMPILE VERTEX SHADER ERROR:\nFILE PATH:" << fragmentFilePath << "\nError:" << msg << std::endl;
	}
	this->programID = glCreateProgram();
	glAttachShader(programID, this->vertexShaderID);
	glAttachShader(this->programID, this->FragmentShaderID);
	glLinkProgram(this->programID);
	glGetProgramiv(this->programID, GL_LINK_STATUS, &error);
	if (!error)
	{
		success = false;
		char msg[128];
		glGetProgramInfoLog(this->programID, 128, NULL, msg);
		std::cout << "Link ERROR:\nError:" << msg << std::endl;
	}
}

bool Shader::useProgram()
{
	if (this->success) {
		glUseProgram(this->programID);
		return true;
	}
	else return false;
}
