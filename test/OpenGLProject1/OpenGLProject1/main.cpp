#include<iostream>
#include<GL/glew.h>
#include<fstream>
#include<sstream>
#include<GLFW/glfw3.h>

using namespace std;
float triangles[] = {
	-0.5f,0.0f,0.0f,
	0.5f,0.f,0.f,
	0.f,0.5f,0.f
};
# define numOfVao 1
# define numOfVbo 1

GLuint getProgram(const char* vert, const char* frag)
{
	ifstream in(vert);
	stringstream s;
	s << in.rdbuf();
	in.close();
	string str_code_vert = s.str();
	in.open(frag);
	s.str("");
	s << in.rdbuf();
	in.close();
	string str_code_frag = s.str();
	const char* c_code_vert = str_code_vert.c_str();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &c_code_vert, NULL);
	glCompileShader(vShader);
	int error;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &error);
	if (error == GL_FALSE)
	{
		char errorMessage[512];
		glGetShaderInfoLog(vShader, 512, NULL, errorMessage);
		cout << "compile vertex shader failed:" << errorMessage << endl;
		exit(EXIT_FAILURE);
	}
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* c_code_frag = str_code_frag.c_str();
	glShaderSource(fShader, 1, &c_code_frag, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &error);
	if (error == GL_FALSE) {
		char errorMessage[512];
		glGetShaderInfoLog(fShader, 512, NULL, errorMessage);
		cout << "compile fragment shader failed:" << errorMessage << endl;
		exit(EXIT_FAILURE);
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &error);
	if (!error)
	{
		char errormessage[512];
		glGetProgramInfoLog(program, 512, NULL, errormessage);
		cout << "link program failed:" << errormessage << endl;
		exit(EXIT_FAILURE);
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	return program;
}

int main(void)
{
	if (glfwInit() == false)
	{
		cout << "glfwinit failed" << endl;
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "texture", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		cout << "glewinit failed" << endl;
		exit(EXIT_FAILURE);
	}
	glViewport(0, 0, 800, 600);
	GLuint vao[numOfVao], vbo[numOfVbo];
	unsigned int program = getProgram("vert.glsl", "frag.glsl");
	glGenBuffers(numOfVbo, vbo);
	glGenBuffers(numOfVao, vao);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	while (glfwWindowShouldClose(window) == false)
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
