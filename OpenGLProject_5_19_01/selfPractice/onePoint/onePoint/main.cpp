#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<fstream>
#include<string>

using namespace std;

enum {
	numberOfVaos = 1,
	numberOfAbos = 1
};
GLuint vao[numberOfVaos];


GLuint createProgram(const char* vertFile, const char* fragFile) {
	ifstream in(vertFile);
	string context;
	string line;
	GLuint fShader, vShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	while (in.eof() == false)
	{
		getline(in, line);
		context.append(line + "\n");
	}
	in.close();
	const char* ch = context.c_str();
	glShaderSource(vShader, 1, &ch, NULL);
	glCompileShader(vShader);
	in.open("fragFile");
	context.clear();
	while (in.eof() == false) {
		getline(in, line);
		context.append(line + "\n");
	}
	in.close();
	ch = context.c_str();
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &ch, NULL);
	glCompileShader(fShader);
	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	cout << glGetError() << endl;

	glGenVertexArrays(numberOfAbos, vao);
	glBindVertexArray(vao[0]);
	return program;
}



int main(void) {
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 400, "point", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	GLuint program = createProgram("frag.glsl", "vert.glsl");
	while (glfwWindowShouldClose(window) == false)
	{
		glUseProgram(program);
		glDrawArrays(GL_POINTS, 0, 1);
		cout << glGetError() << endl;
	}
}