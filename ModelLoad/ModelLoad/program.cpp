#include "program.h"

program::Program::Program(std::string v, std::string f) {
	_id = glCreateProgram();
	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::ifstream in(v.c_str(), std::ios::in);
	std::stringstream ss;
	ss << in.rdbuf();
	std::string v_code = ss.str();
	ss.str("");
	in.close();
	in.open(f.c_str(), std::ios::in);
	ss << in.rdbuf();
	in.close();
	std::string f_code = ss.str();
	const char* vc_code = v_code.c_str();
	const char* fc_code = f_code.c_str();
	glShaderSource(vShader, 1, &vc_code, 0);
	glShaderSource(fShader, 1, &fc_code, 0);
	glCompileShader(vShader);
	int success;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char msg[128];
		glGetShaderInfoLog(vShader, 128, 0, msg);
		std::string wrong = std::string("Compiling vertex shader failed :: ") + std::string(msg);
		throw wrong;
	}
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char msg[128];
		glGetShaderInfoLog(fShader, 128, 0, msg);
		std::string wrong = std::string("Compiling fragment shader failed :: ") + std::string(msg);
		throw wrong;
	}
	glAttachShader(_id, vShader);
	glAttachShader(_id, fShader);
	glLinkProgram(_id);
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		char msg[128];
		glGetProgramInfoLog(_id, 128, 0, msg);
		std::string wrong = std::string("Linking program failed :: ") + std::string(msg);
		throw wrong;
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}
