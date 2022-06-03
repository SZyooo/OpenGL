#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>

using namespace std;

float twoTriangles[] = {
	-0.5f,0,0,
	0,0,0,
	-0.25f,0.5f,0,

	0.5f,0,0,
	0.25f,0.5f,0,
	0,0,0
};

#define numOfVAO 1
#define numOfVBO 1

GLuint createProgram(const char* vertfile,const char* fragfile)
{
	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	string context;
	const char *txt;
	ifstream in(vertfile,ios::in);
	stringstream stream;
	stream << in.rdbuf();
	context = stream.str();
	txt = context.c_str();
	glShaderSource(vShader, 1, &txt, NULL);
	glCompileShader(vShader);
	int error;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &error);
	if (!error) {
		char msg[128];
		glGetShaderInfoLog(vShader, 128, NULL, msg);
		cout << "COMPILATION OF VERTEXT SHADER FAILED:" << msg << endl;
		exit(EXIT_FAILURE);
	}
	stream.str("");
	in.close();
	in.open(fragfile);
	stream << in.rdbuf();
	context = stream.str();
	cout << context;
	txt = context.c_str();
	in.close();
	glShaderSource(fShader, 1, &txt, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &error);
	if (!error)
	{
		char msg[128];
		glGetShaderInfoLog(fShader, 128, NULL, msg);
		cout << "COMPILATION OF FRAGMENT SHADER FAILED:" << msg << endl;
		exit(EXIT_FAILURE);
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glGetProgramiv(program, GL_LINK_STATUS, &error);
	if (!error)
	{
		char msg[128];
		glGetProgramInfoLog(program, 128, NULL, msg);
		cout << "LINK FAIL:" << msg;
		exit(EXIT_FAILURE);
	}
	return program;
}


int main(void)
{
	if (glfwInit() == false)
	{
		cout << "GLFW INIT FAIL" << endl;
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800,600,"two triangle",NULL,NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW INIT FAIL" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	unsigned int VAO, VBO;
	glGenVertexArrays(numOfVAO, &VAO);
	glGenBuffers(numOfVBO, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(twoTriangles), twoTriangles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	GLuint program = createProgram("vert.glsl", "frag.glsl");
	while (glfwWindowShouldClose(window) == false)
	{
		glUseProgram(program);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glfwTerminate();
}